// libsshEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <libssh/libssh.h>
#include <libssh/sftp.h>

int CreateFolder(ssh_session session, sftp_session sftp, char* szFolderName);

#pragma comment(lib, "ssh.lib")

#define S_IRWXU 0000700 
#define S_IRWXG (S_IRWXU >> 3)
#define S_IRWXO (S_IRWXG >> 3)

int main()
{
	ssh_session my_ssh_session;
	sftp_session sftp;

	int verbosity = SSH_LOG_PROTOCOL;
	int port = 9900;
	my_ssh_session = ssh_new();
	if (my_ssh_session == NULL)
		exit(-1);
	ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "localhost");
	ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
	ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);

	int rc;
	sftp = sftp_new(my_ssh_session);
	if (sftp == NULL)
	{
		fprintf(stderr, "Error allocating SFTP session: %s\n",
			ssh_get_error(my_ssh_session));
		return SSH_ERROR;
	}
	rc = sftp_init(sftp);
	if (rc != SSH_OK)
	{
		fprintf(stderr, "Error initializing SFTP session: %s.\n",
			sftp_get_error(sftp));
		sftp_free(sftp);
		return rc;
	}

	CreateFolder(my_ssh_session, sftp, "SampleFolder");
		
	sftp_free(sftp);
	ssh_free(my_ssh_session);
}


int sftp_helloworld(ssh_session session, sftp_session sftp)
{
	int access_type = O_WRONLY | O_CREAT | O_TRUNC;
	sftp_file file;
	const char *helloworld = "Hello, World!\n";
	int length = strlen(helloworld);
	int rc, nwritten;
	
		file = sftp_open(sftp, "helloworld/helloworld.txt",
		access_type, S_IRWXU);
	if (file == NULL)
	{
		fprintf(stderr, "Can't open file for writing: %s\n",
			ssh_get_error(session));
		return SSH_ERROR;
	}
	nwritten = sftp_write(file, helloworld, length);
	if (nwritten != length)
	{
		fprintf(stderr, "Can't write data to file: %s\n",
			ssh_get_error(session));
		sftp_close(file);
		return SSH_ERROR;
	}
	rc = sftp_close(file);
	if (rc != SSH_OK)
	{
		fprintf(stderr, "Can't close the written file: %s\n",
			ssh_get_error(session));
		return rc;
	}
	return SSH_OK;
}

int CreateFolder(ssh_session session, sftp_session sftp, char* szFolderName)
{
	int rc;
	rc = sftp_mkdir(sftp, szFolderName, S_IRWXU);
	if (rc != SSH_OK)
	{
		if (sftp_get_error(sftp) != SSH_FX_FILE_ALREADY_EXISTS)
		{
			fprintf(stderr, "Can't create directory: %s\n",
				ssh_get_error(session));
			return rc;
		}
	}

	return SSH_OK;
}

#define MAX_XFER_BUF_SIZE 16384
int sftp_read_sync(ssh_session session, sftp_session sftp)
{
	int access_type;
	sftp_file file;
	char buffer[MAX_XFER_BUF_SIZE];
	int nbytes, nwritten, rc;
	int fd;
	access_type = O_RDONLY;
	file = sftp_open(sftp, "/etc/profile",
		access_type, 0);
	if (file == NULL) {
		fprintf(stderr, "Can't open file for reading: %s\n",
			ssh_get_error(session));
		return SSH_ERROR;
	}
	/*fd = open("/path/to/profile", O_CREAT);
	if (fd < 0) {
		fprintf(stderr, "Can't open file for writing: %s\n",
			strerror(errno));
		return SSH_ERROR;
	}*/
	for (;;) {
		nbytes = sftp_read(file, buffer, sizeof(buffer));
		if (nbytes == 0) {
			break; // EOF
		}
		else if (nbytes < 0) {
			fprintf(stderr, "Error while reading file: %s\n",
				ssh_get_error(session));
			sftp_close(file);
			return SSH_ERROR;
		}
		/*nwritten = write(fd, buf, nbytes);
		if (nwritten != nbytes) {
			fprintf(stderr, "Error writing: %s\n",
				strerror(errno));
			sftp_close(file);
			return SSH_ERROR;
		}*/
	}
	rc = sftp_close(file);
	if (rc != SSH_OK) {
		fprintf(stderr, "Can't close the read file: %s\n",
			ssh_get_error(session));
		return rc;
	}
	return SSH_OK;
}

// Good chunk size
#define MAX_XFER_BUF_SIZE 16384
int sftp_read_async(ssh_session session, sftp_session sftp)
{
	int access_type;
	sftp_file file;
	char buffer[MAX_XFER_BUF_SIZE];
	int async_request;
	int nbytes;
	long counter;
	int rc;
	access_type = O_RDONLY;
	file = sftp_open(sftp, "some_very_big_file",
		access_type, 0);
	if (file == NULL) {
		fprintf(stderr, "Can't open file for reading: %s\n",
			ssh_get_error(session));
		return SSH_ERROR;
	}
	sftp_file_set_nonblocking(file);
	async_request = sftp_async_read_begin(file, sizeof(buffer));
	counter = 0L;
	Sleep(10000);
	if (async_request >= 0) {
		nbytes = sftp_async_read(file, buffer, sizeof(buffer),
			async_request);
	}
	else {
		nbytes = -1;
	}
	/*while (nbytes > 0 || nbytes == SSH_AGAIN) {
		if (nbytes > 0) {
			write(1, buffer, nbytes);
			async_request = sftp_async_read_begin(file, sizeof(buffer));
		}
		else {
			counter++;
		}
		usleep(10000);
		if (async_request >= 0) {
			nbytes = sftp_async_read(file, buffer, sizeof(buffer),
				async_request);
		}
		else {
			nbytes = -1;
		}
	}
	if (nbytes < 0) {
		fprintf(stderr, "Error while reading file: %s\n",
			ssh_get_error(session));
		sftp_close(file);
		return SSH_ERROR;
	}*/
	printf("The counter has reached value: %ld\n", counter);
	rc = sftp_close(file);
	if (rc != SSH_OK) {
		fprintf(stderr, "Can't close the read file: %s\n",
			ssh_get_error(session));
		return rc;
	}
	return SSH_OK;
}

int sftp_list_dir(ssh_session session, sftp_session sftp)
{
	sftp_dir dir;
	sftp_attributes attributes;
	int rc;
	dir = sftp_opendir(sftp, "/var/log");
	if (!dir)
	{
		fprintf(stderr, "Directory not opened: %s\n",
			ssh_get_error(session));
		return SSH_ERROR;
	}
	printf("Name                       Size Perms    Owner\tGroup\n");
	while ((attributes = sftp_readdir(sftp, dir)) != NULL)
	{
		printf("%-20s %10llu %.8o %s(%d)\t%s(%d)\n",
			attributes->name,
			(long long unsigned int) attributes->size,
			attributes->permissions,
			attributes->owner,
			attributes->uid,
			attributes->group,
			attributes->gid);
		sftp_attributes_free(attributes);
	}
	if (!sftp_dir_eof(dir))
	{
		fprintf(stderr, "Can't list directory: %s\n",
			ssh_get_error(session));
		sftp_closedir(dir);
		return SSH_ERROR;
	}
	rc = sftp_closedir(dir);
	if (rc != SSH_OK)
	{
		fprintf(stderr, "Can't close directory: %s\n",
			ssh_get_error(session));
		return rc;
	}

	return 0;
}