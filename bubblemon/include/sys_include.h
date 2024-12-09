/*  BubbleMon dockapp 1.2
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Street #330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef _SYS_INCLUDE_H_
#define _SYS_INCLUDE_H_

/* these routines must be written for each new platform */
int system_cpu(void);		/* return total CPU load in percent */
int system_memory(void);	/* set memory related values in BubbleMonData
				   and return 1, or return 0 if memory didn't
				   change */
#ifdef ENABLE_MEMSCREEN
void system_loadavg(void);	/* get current load average and put into
				   bm->loadavg[].{i,f} */
#endif
#endif /* _SYS_INCLUDE_H_ */
