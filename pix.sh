#chmod 644 /etc/profile.d/pix.sh
if [[ "$(ps -p $PPID -o comm=)" == "pix" ]]; then
	PS1="\033[0;32m${PS1}\033[0m"
fi
