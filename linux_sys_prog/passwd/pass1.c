// Gerneral Lib
#include <stdio.h>
#include <string.h>
// Linux Lib
#include <sys/types.h>
#include <pwd.h>
#include <shadow.h> // Super User Open

//struct passwd *getpwent(void);

int main(int argc, char *argv[])
{
	char *name;
	if(argc!=2)
	{
		printf("Usage : %s username\n", argv[0]);
		return 1;
	}
	name = argv[1];

	struct passwd *pass_info = NULL;
	while( (pass_info = getpwent()) !=NULL)
	{
		if(strncmp(name, pass_info -> pw_name, strlen(name)) ==0)
		{
			printf("%12s uid(%d) gid(%d) home(%s)\n",
					pass_info->pw_name,
					pass_info->pw_uid,
					pass_info->pw_gid,
					pass_info->pw_dir);
			printf("\r\n name_size[%lx]",sizeof(name));
		}
	}

	// User Password Read ( But Super User Excute)
	struct spwd *spwd_info = NULL;
	while( (spwd_info = getspent())!=NULL)
	{
		printf( "%-20s%s\n", spwd_info->sp_namp,spwd_info->sp_pwdp);
	}
	spwd_info=getspnam(argv[1]);
        printf( "%-20s%s\n", spwd_info->sp_namp,spwd_info->sp_pwdp);
	printf("Can not find user %s\n", name);
	return 1;
}
