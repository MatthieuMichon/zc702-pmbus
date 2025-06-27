package require Vivado

set vivado_version [version -short]

proc build_2025_1 {} {

    create_project -force -part xc7z020clg484-1 pmbus_zc702
    set_property -dict [ list \
        BOARD_PART xilinx.com:zc702:part0:1.4 \
        SOURCE_MGMT_MODE All] [current_project]

    foreach bd [glob ../bd_*_2025_1.tcl] {source ${bd}}
    set bd_files [get_files -filter "NAME=~*.bd && IS_GENERATED==0"]
    set_property synth_checkpoint_mode None ${bd_files}
    generate_target all ${bd_files}
    export_ip_user_files \
        -of_objects ${bd_files} \
        -no_script -sync -force -quiet
    compile_c [get_ips]

    foreach sv [glob ../*.sv] {add_files ${sv}}
    set top [lindex [find_top] 0]
    
    launch_runs synth_1 -jobs 16
    wait_on_run synth_1

    set_property -dict [list \
        steps.opt_design.args.verbose 1 \
        {steps.opt_design.args.more options} -debug_log \
        {steps.place_design.args.more options} {-debug_log -verbose} \
        {steps.route_design.args.more options} {-tns_cleanup -debug_log -verbose} \
    ] [get_runs impl_1]
    launch_runs impl_1 -jobs 16
    wait_on_run impl_1
    open_run impl_1
    launch_runs impl_1 -to_step write_bitstream -jobs 16
    wait_on_run impl_1
    write_hw_platform -fixed -include_bit -force -file pmbus_zc702.xsa
}

switch ${vivado_version} {
    2025.1 { build_2025_1 }
    default { common::send_gid_msg -ssname BD::TCL -id 1000 -severity "ERROR" " Usupported ${current_vivado_version} version of Vivado." } 
}
