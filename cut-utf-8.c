/* 切分 UTF-8
 *
 * Example: $ echo _A_Ə_中_𐍈_ | cut-utf-8 5
 *           中_𐍈_
 */

#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      fprintf (stderr,
               "Usage: %s N\n"
               "删除 STDIN 中每行的前 N 个 UTF-8 字符\n",
               argv[0]);
      return -1;
    }

  int N = strtol (argv[1], NULL, 10);

  int i = 1;
  int c;
  while ((c = getc (stdin)) != EOF)
    {
      if (c == '\n')
        {
          i = 1;
          putc (c, stdout);
          continue;
        }
      if ((c >> 7) == 0)
        {
          if (i > N) putc (c, stdout);
        }
      else if ((c >> 5) == 6)
        {
          int c1 = getc (stdin);
          if (i > N)
            {
              putc (c, stdout);
              putc (c1, stdout);
            }
        }
      else if ((c >> 4) == 14)
        {
          int c1 = getc (stdin);
          int c2 = getc (stdin);
          if (i > N)
            {
              putc (c, stdout);
              putc (c1, stdout);
              putc (c2, stdout);
            }
        }
      else if ((c >> 3) == 30)
        {
          unsigned char rest[3];
          fread (rest, 1, 3, stdin);
          if (i > N)
            {
              putc (c, stdout);
              fwrite (rest, 1, 3, stdout);
            }
        }
      i++;
    }
  return 0;
}
