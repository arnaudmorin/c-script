#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>

const struct pam_conv conv = {
    misc_conv,
    NULL
};

extern char **environ;
extern int errno;

int main (int argc, char *argv[]) {
    pam_handle_t* pamh = NULL;
    uid_t uid, euid;
    int retval;
    const char* user = "nobody";
    
    if(argc != 2) {
        printf("Usage: app [username]\n");
        exit(1);
    }
    
    user = argv[1];
    
    retval = pam_start("check_user", user, &conv, &pamh);

    // Are the credentials correct?
    if (retval == PAM_SUCCESS) {
        printf("Credentials accepted.\n");
        retval = pam_authenticate(pamh, 0);
    }

    // Can the accound be used at this time?
    if (retval == PAM_SUCCESS) {
        printf("Account is valid.\n");
        retval = pam_acct_mgmt(pamh, 0);
    }

    // Did everything work?
    if (retval == PAM_SUCCESS) {
        printf("Authenticated\n");
        uid=getuid();
        euid=geteuid();
        setreuid (euid, euid);
        execve("/bin/bash", NULL, environ);
        printf ("Error : %d\n", errno);
    } else {
        printf("Not Authenticated\n");
    }


    // close PAM (end session)
    if (pam_end(pamh, retval) != PAM_SUCCESS) {
        pamh = NULL;
        printf("check_user: failed to release authenticator\n");
        exit(1);
    }

    return errno;
}
