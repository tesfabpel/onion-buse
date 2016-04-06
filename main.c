#define _GNU_SOURCE
#define _LARGEFILE64_SOURCE

#include <stdlib.h>
#include <libgen.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "buse.h"

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

static int ofd;
static int sfd;
static int vfd;

static void usage(void)
{
	fprintf(stderr, "Usage: onionbuse <ro file> [<snapshot file>] <virtual file>\n");
}

static int onion_read(void *buf, u_int32_t len, u_int64_t offset, void *userdata)
{
	// TODO!

	// TODO: Read from log

	ssize_t bytes_read;
	(void)(userdata);

	off64_t ooff = lseek64(ofd, offset, SEEK_SET);

	while (len > 0) {
		bytes_read = read(ofd, buf, len);
		assert(bytes_read > 0);
		len -= bytes_read;
		buf = (char *) buf + bytes_read;
	}

	return 0;
}

static int onion_write(const void *buf, u_int32_t len, u_int64_t offset, void *userdata)
{
	// TODO!

	// TODO: Write to snapshot and log

	ssize_t bytes_written;
	(void)(userdata);

	lseek64(ofd, offset, SEEK_SET);
	while (len > 0) {
		bytes_written = write(ofd, buf, len);
		assert(bytes_written > 0);
		len -= bytes_written;
		buf = (char *) buf + bytes_written;
	}

	return 0;
}

static struct buse_operations onion_bop = {
	.read = onion_read,
	.write = onion_write
};

char *file_add_suffix(char *orig, const char *suffix)
{
	char *dir = dirname(orig);
	char *fn = basename(orig);

	size_t len = 0;
	len += strlen(dir);
	len += 1; // path separator
	len += strlen(fn);
	len += strlen(suffix);
	len += 1; // NUL

	char *ofile = malloc(sizeof(char) * len); // freed by the OS at the end
	strcpy(ofile, dir);
	strcat(ofile, "/");
	strcat(ofile, fn);
	strcat(ofile, suffix);

	return ofile;
}

int main(int argc, char* argv[])
{
	char *original_file = NULL;
	char *snapshot_file = NULL;
	char *log_file = NULL;
	char *virtual_file = NULL;

	if (argc != 4 && argc != 3)
	{
		usage();
		return -1;
	}

	if(argc == 3)
	{
		original_file = argv[1];
		virtual_file = argv[2];
	}
	else
	{
		original_file = argv[1];
		snapshot_file = argv[1];
		virtual_file = argv[3];
	}

	if(snapshot_file == NULL)
	{
		// Make snapshot_file to be: foo/bar/orig.img.snap
		snapshot_file = file_add_suffix(original_file, ".snap");
	}

	log_file = file_add_suffix(snapshot_file, ".log");

	// TODO: Try to open and create files

	// TODO: Run BUSE

	return 0;
}
