#!/usr/bin/awk -f
#导包

BEGIN{
	#自带内置变量
	FS=":"
	nt="NO";
	ut="USER";
	it="USER_ID";
	ht="HOME_PATH";
	tt="TTY";
	i=0;
	printf "%-10s%-30s%-10s%-30s%-10s\n",nt, ut, it, ht, tt;
}
{
	if($3>300)
	printf "%-3d%-30s%-10s%-10s%-10s\n",++i, $1, $3, $4, $7;
}
END{
	printf "exit..\n";
}
