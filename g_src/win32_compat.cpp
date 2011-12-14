#include <string>

#include "enabler.h"
#include "platform.h"
#ifndef WIN32
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <signal.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
#endif

#ifndef WIN32
BOOL CreateDirectory(const char* pathname, void*)
{
  if (mkdir(pathname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) {
    if (errno != EEXIST) {
      std::string emsg = "mkdir(";
      emsg.append(pathname);
      emsg.append(") failed");
      perror(emsg.c_str());
    }
    return FALSE;
  } else {
    return TRUE;
  }
}

BOOL DeleteFile(const char* filename)
{
  return !unlink(filename);
}

void ZeroMemory(void* dest, int len)
{
  memset(dest, 0, len);
}

/* Returns milliseconds since 1970
 * Wraps every 24 days (assuming 32-bit signed dwords)
 */
DWORD GetTickCount()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

char* itoa(int value, char* result, int base)
{
  // check that the base is valid
  if (base < 2 || base > 16) { *result = 0; return result; }

  char* out = result;
  int quot = value;

  do
    {
      *out = "0123456789abcdef"[ /*std::*/abs(quot % base) ];
      ++out;
      quot /= base;
    }
  while (quot);

  if (value < 0) *out++ = '-';

  std::reverse(result, out);
  *out = 0;
  return result;
}

// Fills performanceCount with microseconds passed since 1970
// Wraps in twenty-nine thousand years or so
BOOL QueryPerformanceCounter(LARGE_INTEGER* performanceCount)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  performanceCount->QuadPart = ((long long)tp.tv_sec * 1000000) + tp.tv_usec;
  return TRUE;
}

BOOL QueryPerformanceFrequency(LARGE_INTEGER* performanceCount)
{
  /* A constant, 10^6, as we give microseconds since 1970 in
   * QueryPerformanceCounter. */
  performanceCount->QuadPart = 1000000;

  return TRUE;
}

#endif
