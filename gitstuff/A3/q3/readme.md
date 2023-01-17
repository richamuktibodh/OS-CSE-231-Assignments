# Q3
__If no command line argument entered in command line, default value of pidval is 1.__

I haven't added a syscall to the kernel using this module, this module takes the process id as input, and returns the **user_id**, **process_ id**, **process group id**.

_Commands required to run file_

__dmesg --clear__

__lsmod --> lists all current modules__

__make --> compiles file__

__insmod testmod.ko pidval=<<process_id>> --> loads module into kernel__

__dmesg --> shows kernel logs__

__rmmod testmod.ko --> removes module__

__dmesg__

### Reference
https://elixir.bootlin.com/linux/v5.19.9/source/include/linux/sched.h#L726










