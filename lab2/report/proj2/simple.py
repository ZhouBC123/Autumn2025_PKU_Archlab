import m5
from m5.objects import *
import argparse


cpu_types = {
    "simple": TimingSimpleCPU,
    "minor": MinorCPU,
    "o3": O3CPU,
}
mem_types = {
    "DDR3": DDR3_1600_8x8,
    "DDR4": DDR4_2400_8x8,
    "DDR5": DDR5_8400_4x8,
}
bp_types = {
    "local": LocalBP,
    "bimode": BiModeBP,
    "tournament": TournamentBP,
    "simple": SimpleBP,
    "perceptron": Lab2PerceptronBP
}

def init_system(system):
    # system.clk_domain = SrcClockDomain(clock='4GHz', voltage_domain=VoltageDomain())
    # DELETE: system.clk_domain = SrcClockDomain(clock='4GHz', voltage_domain=VoltageDomain())
    system.clk_domain = SrcClockDomain(clock=args.clock, voltage_domain=VoltageDomain())

    system.mem_mode = 'timing'
    system.mem_ranges = [AddrRange ('2GB')]
    system.mem_ctrl = MemCtrl()
    # system.mem_ctrl.dram = DDR4_2400_8x8()
    # DELETE: system.mem_ctrl.dram = DDR4_2400_8x8()
    system.mem_ctrl.dram = mem_types[args.mem]()
    system.mem_ctrl.dram.range = root.system.mem_ranges[0]
    # system.cpu = TimingSimpleCPU()
    # DELETE: system.cpu = TimingSimpleCPU()
    system.cpu = cpu_types[args.cpu]()
    system.membus = SystemXBar()

    system.cpu.icache_port = system.membus.cpu_side_ports
    system.cpu.dcache_port = system.membus.cpu_side_ports
    system.mem_ctrl.port = system.membus.mem_side_ports
    system.cpu.createInterruptController()
    system.system_port = system.membus.cpu_side_ports
    # system.cpu.interrupts[0].pio = system.membus.mem_side_ports
    # system.cpu.interrupts[0].int_requestor = system.membus.cpu_side_ports
    # system.cpu.interrupts[0].int_responder = system.membus.mem_side_ports

    system.cpu.branchPred = bp_types[args.bp]()

    # recommended: 1e+8 or 2e+8
    system.cpu.max_insts_any_thread = 5e+8

import os
# DELETE: def init_process(root):
def init_process(root, args):
    # DELETE: exe_path = 'tests/labexe/hello'
    exe_path = args.commands_to_run[0]
    root.system.workload = SEWorkload.init_compatible(exe_path)
    process = Process()
    process.executable = exe_path
    process.cwd = os.getcwd()
    # DELETE: process.cmd = [exe_path]
    process.cmd = args.commands_to_run
    root.system.cpu.workload = process
    root.system.cpu.createThreads()



if __name__ == '__m5_main__':

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "commands_to_run",
        nargs="*",
        help="Command(s) to run",
    )
    parser.add_argument(
        "--cpu",
        type=str,
        choices=list(cpu_types.keys()),
        default="o3",
        help="CPU model to use",
    )
    parser.add_argument(
        "--mem",
        type=str,
        choices=list(mem_types.keys()),
        default="DDR4",
        help="type of memory to use",
    )
    parser.add_argument(
        "--clock", 
        type=str, 
        default="2GHz"
    )
    parser.add_argument(
        "--bp",
        type=str,
        choices=list(bp_types.keys()),
        default="tournament",
        help="branch predictor to use",
    )
    args = parser.parse_args()

    root = Root()
    root.full_system = False
    root.system = System()

    init_system(root.system)
    init_process(root, args)
    m5.instantiate()
    exit_event = m5.simulate()
    print(f'{exit_event.getCause()} ({exit_event.getCode()}) @ {m5.curTick()}')