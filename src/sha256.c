#include "cheese.h"

static uint32_t H[8] = {
	0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
	0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
};

static uint32_t K[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static inline uint8_t get_byte(const uint8_t *str, size_t pos, size_t len) {
	if (pos < len)
		return str[pos];
	if (pos == len)
		return 0b10000000;

	size_t null_start = len + 1;
	size_t null_end = null_start;
	while (null_end % (512 / 8) != (448 / 8))
		null_end++;
	if (pos < null_end)
		return 0;
	if (pos >= null_end)
		pos -= null_end;

	uint64_t len_u64 = 8 * (uint64_t)len;

	uint8_t len_arr[8];
	memcpy(len_arr, &len_u64, 8);
	if (pos > 7)
		return 0;
	#ifdef BIG_ENDIAN
		return len_arr[pos];
	#else
		return len_arr[7 - pos];
	#endif
}

static void get_chunk(const uint8_t *str, size_t chunk_num, size_t len, uint8_t chunk[64]) {
	size_t chunk_pos = chunk_num * 64;
	for (int i = 0; i < 64; i++)
		chunk[i] = get_byte(str, chunk_pos++, len); 
}

#define ROTR(X, N) (((X) >> (N)) | ((X) << (32 - (N))))
#define SHR(X, N)  ((X) >> (N))

#define EP0(X) (ROTR(X,2) ^ ROTR(X,13) ^ ROTR(X,22))
#define EP1(X) (ROTR(X,6) ^ ROTR(X,11) ^ ROTR(X,25))

#define SIG0(X) (ROTR(X,7) ^ ROTR(X,18) ^ SHR(X,3))
#define SIG1(X) (ROTR(X,17) ^ ROTR(X,19) ^ SHR(X,10))

#define CH(X,Y,Z)  (((X) & (Y)) ^ (~(X) & (Z)))
#define MAJ(X,Y,Z) (((X) & (Y)) ^ ((X) & (Z)) ^ ((Y) & (Z)))


static void process_chunk(uint8_t chunk[64], uint32_t hash[8]) {
	uint32_t words[64] = {0};

	for (int i = 0; i < 16; i++) {
		words[i] = ((uint32_t)chunk[4 * i] << 24) | ((uint32_t)chunk[4 * i + 1] << 16)
				| ((uint32_t)chunk[4 * i + 2] << 8) | chunk[4 * i + 3];
	}
	for (int i = 16; i < 64; i++)
		words[i] = SIG1(words[i - 2]) + words[i - 7] + SIG0(words[i - 15]) + words[i - 16];
	
	uint32_t a = hash[0];
	uint32_t b = hash[1];
	uint32_t c = hash[2];
	uint32_t d = hash[3];
	uint32_t e = hash[4];
	uint32_t f = hash[5];
	uint32_t g = hash[6];
	uint32_t h = hash[7];
	for (int i = 0; i < 64; i++) {
		uint32_t t1 = h + EP1(e) + CH(e, f, g) + K[i] + words[i];
		uint32_t t2 = EP0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}
	hash[0] += a;
	hash[1] += b;
	hash[2] += c;
	hash[3] += d;
	hash[4] += e;
	hash[5] += f;
	hash[6] += g;
	hash[7] += h;
}


char *sha256(char *str) {
	char *res = malloc(65);
	if (!res)
		return NULL;
	size_t len = strlen(str);
	size_t total_len = len;
	while (total_len % (512 / 8) != (448 / 8))
		total_len++;
	total_len += 8;

	uint32_t hash[8] = {0};
	memcpy(hash, H, sizeof(uint32_t) * 8);
	
	for (size_t i = 0; i < (len + 1 + 8 + 63) / 64; i++) {
		uint8_t chunk[64];
		get_chunk((uint8_t *)str, i, len, chunk);
		process_chunk(chunk, hash);
	}
	uint8_t big_hash[32];
	for (int i = 0; i < 8; i++) {
		big_hash[4 * i] = (hash[i] >> 24 ) & 0xff;
		big_hash[4 * i + 1] = (hash[i] >> 16) & 0xff;
		big_hash[4 * i + 2] = (hash[i] >> 8) & 0xff;
		big_hash[4 * i + 3] = hash[i] & 0xff;
	}
	for (int i = 0; i < 32; i++) {
		char *base = "0123456789abcdef";
		char high = base[big_hash[i] >> 4];
		char low = base[big_hash[i] & 0x0f];
		sprintf(&res[i * 2], "%c%c", high, low);
	}
	res[64] = '\0';
	return res;
}
