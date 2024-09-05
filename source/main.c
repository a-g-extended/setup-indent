#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#ifndef O_BINARY
#define O_BINARY 0
#endif

int
main (int argc, char **argv)
{
  char *filename;
  int fd;
  struct stat st;
  size_t filesize;
  char *rp;
  char *wp;
  size_t i;
  if (argc != 0 && argc != 1)
    {
      filename = argv[1];
      fd = open (filename, O_BINARY | O_RDWR);
      if (fd != -1)
	{
	  fstat (fd, &st);
	  filesize = st.st_size;
	  rp =
	    mmap (NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	  if (rp == MAP_FAILED)
	    {
	      close (fd);
	    }
	  else
	    {
	      wp = rp;
	      for (i = 0; i != filesize; i++)
		{
		  if (*rp == ' ' || *rp == '\t')
		    {
		      *wp = ' ';
		      wp++;
		      rp++;
		      i++;
		    }
		  while (*rp == ' ' || *rp == '\t')
		    {
		      rp++;
		      i++;
		    }
		  *wp = *rp;
		  wp++;
		  rp++;
		}
	      close (fd);
	      munmap (rp - i, filesize);
	      truncate (filename, filesize - (((size_t) rp) - ((size_t) wp)));
	    }
	}
    }
  return 0;
}
