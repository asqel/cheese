#include <string.h>
#include <stdlib.h>

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

char	*strjoin(char *s1, char *s2, int free_s1)
{
	char	*dest;

	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (s1);

	size_t s1_len = strlen(s1);
	size_t s2_len = strlen(s2);
	dest = calloc((s1_len + s2_len + 1), sizeof(char));
	if (!dest)
		return (NULL);
	strcat(dest, s1);
	strcat(dest + s1_len, s2);
	dest[s1_len + s2_len] = 0;
	if (free_s1)
		free(s1);
	return (dest);
}
