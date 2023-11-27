set pagination off
set logging file gdb.txt
set logging on

set architecture arm
set endian little
set arm fallback-mode thumb
set arm force-mode thumb
set style enabled off
file /home/sz/work/nitrokey-pro-firmware/build/gcc/nitrokey-hsm-firmware.elf
target remote :3333

define trun
	monitor halt
	b smartcard.c:529	
	commands 
		ttest 0x33 # $arg0
	end
	monitor reset
	shell sleep 1
	c
end

define ttest
	shell sleep 1
	p g_test_value
	set var g_test_value=$arg0
	p g_test_value
	c
end

b Default_Handler
commands
	bt
end


	
 
define stepinf
    while(1)
        p $msp
    step
    end
end

# Automatically start tracing upon reaching the following line
# b transport.rs:181
# commands
# stepinf
# end
monitor reset halt
# b smartcard.c:529
# b SC_PTSConfig
#c
