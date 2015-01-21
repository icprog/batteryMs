# This configuration script is compatible with MPC563xM MCUs.
# Features:
# - Autodetects the current MPC563xM part
# - Disables the Software Watchdog Timer (SWT)
# - Initializes the MMU for the z3 core
# - Initializes the system ECC internal SRAM 
# - Sets the PC to the user program entry point
#
# VERSION: 1.1

######################################
# Initialize target variables
######################################
# var booke_vle: BOOKE = booke, VLE = vle
set booke_vle vle
 
#################################################
# Initialize a RAM 'range' from 'start' address,
# downloading the init program at 0x4000_0000.
#################################################
proc init_ram {start range} {
  	# GPR registrer group
  	set GPR_GROUP "General Purpose Registers/"
    global booke_vle

    puts "init ECC SRAM $start:$range"
    # default SRAM address
    set pstart 0x40000000
    # the offset from the given start address
    # at which the init prgram starts to init SRAM
    set offset 0x0
    # stmw write page size = 128 = 4bytes * 32 GPRS
    set psize 0x80

    if {$start == $pstart} {
        # init first 4 bytes (mem access) x 128 = 512
        # bytes to avoid reading the memory around PC
        # after stopping the core
        mem $start 256 = 0x0
        # base init address
        set offset 0x80
    }

    # address to start initialization
    set start [expr {$start + $offset}]
    
    # load add into GPR
    reg ${GPR_GROUP}GPR11 %d = $start

    # compute actual times stmw is called
    # and set counter
    set c [expr ((($range - $offset)/$psize))]
    reg ${GPR_GROUP}GPR12 %d = $c

    # execute init ram code
    if {$booke_vle == "vle"} {
        #mtctr r12
        mem $pstart = 0x7D8903A6
        #stmw r0,0(r11)
        mem [format %x [expr $pstart + 0x4]] = 0x180B0900
        #addi r11,r11,128
        mem [format %x [expr $pstart + 0x8]] = 0x1D6B0080
        #bdnz -8
        mem [format %x [expr $pstart + 0xc]] = 0x7A20FFF8
        # infinte loop
        #se_b *+0
        mem [format %x [expr $pstart + 0x10]] = 0xE8000000
    } else {
        #mtctr r12
        mem $pstart = 0x7D8903A6
        #stmw r0,0(r11)
        mem [format %x [expr $pstart + 0x4]] = 0xBC0B0000
        #addi r11,r11,128
        mem [format %x [expr $pstart + 0x8]] = 0x396B0080
        #bdnz -8
        mem [format %x [expr $pstart + 0xc]] = 0x4200FFF8
        # infinte loop
        #se_b *+0
        mem [format %x [expr $pstart + 0x10]] = 0x48000000    
    }

    # set PC to the first init instruction
    reg ${GPR_GROUP}PC = $pstart
    # execute init ram code
    # timeout 1 second to allow the code to execute
    go 1
    stop
    if {[reg ${GPR_GROUP}PC -np] != [expr $pstart + 0x10]} {
        puts stderr "Exception occured during SRAM initialization."
    } else {
        puts "SRAM initialized."
    }
}

################################################
# Initialize the Z3 core registers: enable SPE
# reset watchdog timer, invalidate stack pointer
################################################
proc init_z3 {} {
  	# GPR registrer group
  	set GPR_GROUP "General Purpose Registers/"
  	# special purpose register group
  	set SPR_GROUP "e200z3 Special Purpose Registers/"
	global booke_vle
  	#TLB1 registers group
  	variable TLB1_GROUP "regPPCTLB1/"

	puts "init Core0 Z3"
    # set SP to an unaligned (4bytes) to avoid creating
	# invalid stack frames
    reg ${GPR_GROUP}SP = 0x3 
   	# reset watchdog timer
	change ${SPR_GROUP}TCR = 0x0
	#enable SPE
	change ${SPR_GROUP}MSR = 0x02000200

  	if {$booke_vle == "vle"} {
	  	reg ${TLB1_GROUP}MMU_CAM0 = 0x5000000afe080000fff00000fff00001
	  	reg ${TLB1_GROUP}MMU_CAM1 = 0x70000000fe0800000000000000000001
	  	reg ${TLB1_GROUP}MMU_CAM3 = 0x40000008fe0800004000000040000001
	  	reg ${TLB1_GROUP}MMU_CAM4 = 0x5000000afe080000c3f00000c3f00001
  	} else {
	  	reg ${TLB1_GROUP}MMU_CAM0 = 0x5000000afc080000fff00000fff00001
	  	reg ${TLB1_GROUP}MMU_CAM1 = 0x70000000fc0800000000000000000001
	  	reg ${TLB1_GROUP}MMU_CAM3 = 0x40000008fc0800004000000040000001
	  	reg ${TLB1_GROUP}MMU_CAM4 = 0x5000000afc080000c3f00000c3f00001
  	}

    # Initialize core interrupts in order to catch any exceptions during
    # MCU initialization. The following sequence queries symbolics to
    # obtain the address of the wellknown symbol "ivor_branch_table".
    set ivor_table 0x40000000
    set r [catch {set ivor_table [evaluate ivor_branch_table]}]
    if {$r == 0} {
        # set IVPR vector base table high 16-bit and IVOR0 lower 16-bit
        set ivor0 0x[format %x [expr $ivor_table & 0xFFFF]]
        reg ${SPR_GROUP}IVPR = 0x[format %x [expr $ivor_table & 0xFFFF0000]]
        # Initiliaze only the first 16 IVORs
        for {set i 0} {$i < 16} {incr i} {
            # The IVOR branch table must have a 16 byte aligment.
            reg ${SPR_GROUP}IVOR${i} = 0x[format %x [expr $ivor0 + $i * 0x10]]
        }
    }
}

#################################################################
# Tries to obtain the program entry point from binary debug info
# or by searching for the first flash boot sector. If neither
# is succesfull it returns the BAM start address.
#################################################################
proc get_entry_point {} {
    puts "Get application entry point ..."
    # check symbolic names first
    set reset_vector_addr 0x3
    catch {set reset_vector_addr [evaluate __startup]}
    # If no debug info available search for a FLASH boot sector
    if {$reset_vector_addr == 0x3} {
        # RCHW addresses of each Flash boot sector
        set SECTORS_ADD [list 0x0000 0x4000 0x10000 0x1C000 0x20000 0x30000]
        puts "Searching for boot sectors ..."
        set rchw_value 0x0
        foreach rchw $SECTORS_ADD {
            catch {set rchw_value [mem $rchw -np]}
            if {[expr $rchw_value & 0xFF0000] == 0x5A0000} {
                catch {set reset_vector_addr [mem [expr $rchw + 4] -np]}
                set reset_vector_addr [format "0x%x" [expr $reset_vector_addr & 0xFFFFFFFF]]
                puts "Found boot sector at $rchw and entry point at $reset_vector_addr."  
                break
            }
        }        
    }
    if {$reset_vector_addr == 0x3} {
        puts "No entry point available. Set entry point to BAM start address."
        set reset_vector_addr 0xFFFFFFFC
    }
    return $reset_vector_addr
}  

##############################################################
## Detects the current MPC563xM device and returns de RAM size
##############################################################
proc get_mem_size {} {
    set SIUL_MIDR1 0x0
    set SIUL_MIDR2 0x0

    # SRAM memory sizes for supported devices.
    # $mem_size($current_cfg) gives the current RAM to init
    # CFG = x from MPC563xM
    # CFG         Mem Size
    # ------------------------
    # 5632         48K (0xC000)
    # 5633         64K (0x10000)
    # 5634         96K (0x17800)
    # ------------------------
    set mem_size(5632) 0xC000
    set mem_size(5633) 0x10000
    set mem_size(5634) 0x17800

    # default detivative
    set current_cfg 5634

    puts "reading SIUL.MIDR1 ..."
    set r [catch {set SIUL_MIDR1 [mem 0xC3F90004]}]
    if {$r == 0} {
        set r [expr ($SIUL_MIDR1 & 0xFFF00000) == 0x56300000]
        if {$r == 1} {
            set partnum1 [expr ($SIUL_MIDR1 & 0xFFFF0000) >> 16]
            puts "reading SIUL.MIDR2 ..."
            set SIUL_MIDR2 [mem 0xC3F90000]
            set partnum2 [expr ($SIUL_MIDR2 & 0x0000FF00) >> 8]
			# char M = 0x4D = 77
            if {$partnum2 == 77} {
                set current_cfg [format %x $partnum1]
            } else {
                puts "Wrong partnum in MIDR2 or MPC534xC MCU."
            }
            puts [format "Current device: MPC%s, mask 0x%x" $current_cfg [expr $SIUL_MIDR1 & 0x000000FF]]
        } else {
            puts [format "Incompatible device: MPC%x. Use default device MPC%s" [expr ($SIUL_MIDR1 & 0xFFFF0000) >> 16] $current_cfg]
        }
    } else {
        puts "Unable to detect the device."
    }
    return $mem_size($current_cfg)
}

proc mpc563xm_init {} {
	set SPR_GROUP "e200z3 Special Purpose Registers/"
	set GPR_GROUP "General Purpose Registers/"
	global booke_vle
	
	# Explicitly stop Core0
	stop
	
	#initialize Core0
	init_z3

	# Disable Watchdog Timers
	mem v:0xFFF38010 = 0x0000C520
	mem v:0xFFF38010 = 0x0000D928
	mem v:0xFFF38000 = 0x8000010A
	
    # detect MPC563xM device & initialze ECC SRAM    
    init_ram 0x40000000 [get_mem_size]

	# initialize PC after reset with application start address or BAM otherwise
    reg ${GPR_GROUP}PC = [get_entry_point]
    # set SP to an unaligned (4bytes) to avoid creating invalid stack frames
    reg ${GPR_GROUP}SP = 0x3
}

proc envsetup {} {
	# Environment Setup
	radix x 
	config hexprefix 0x
	config MemIdentifier v 
	config MemWidth 32 
	config MemAccess 32 
	config MemSwap off
}

#-------------------------------------------------------------------------------
# Main                                                                          
#-------------------------------------------------------------------------------
   	
envsetup

mpc563xm_init
