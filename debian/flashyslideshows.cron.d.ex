#
# Regular cron jobs for the flashyslideshows package
#
0 4	* * *	root	[ -x /usr/bin/flashyslideshows_maintenance ] && /usr/bin/flashyslideshows_maintenance
