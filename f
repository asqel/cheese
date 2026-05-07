==8958== Helgrind, a thread error detector
==8958== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==8958== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==8958== Command: ./cheese -c client_config
==8958== 
==8958== 
==8958== Process terminating with default action of signal 2 (SIGINT)
==8958==    at 0x4962ADF: clock_nanosleep@@GLIBC_2.17 (clock_nanosleep.c:78)
==8958==    by 0x496FA26: nanosleep (nanosleep.c:25)
==8958==    by 0x499F75B: usleep (usleep.c:31)
==8958==    by 0x115A3A: test (main_menu.c:51)
==8958==    by 0x115A8B: clt_main_menu (main_menu.c:60)
==8958==    by 0x11585F: launch_client (launch.c:23)
==8958==    by 0x10F445: main (main.c:54)
==8958== 
==8958== Use --history-level=approx or =none to gain increased speed, at
==8958== the cost of reduced accuracy of conflicting-access information
==8958== For lists of detected and suppressed errors, rerun with: -s
==8958== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 3887 from 24)
