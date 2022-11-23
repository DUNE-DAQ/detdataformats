#!/usr/bin/env python3
import click
import socket
import time
from rich.console import Console
import detdataformats

console = Console()

CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])
@click.command(context_settings=CONTEXT_SETTINGS)
@click.option('-p','--port', default=1234, help='Port number')


def cli(port):
    console.log(f"Preparing to receive WIB packets on port {port}.")
    sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
    sock.bind(("0.0.0.0", port))

    rcv_bytes = 0
    rcv_pkts_tot  = 0
    rcv_pkts  = 0
    rcv_bytes = 0
    start_time = time.time();
    seq_checker = [0,0,0,0]
    while True:
        try:
            data, addr = sock.recvfrom(8192)
            rcv_bytes += len(data)
            wf = detdataformats.wibeth.WIBEthFrame(data)
            if seq_checker[wf.get_daqheader().stream_id%4]+1 != wf.get_daqheader().seq_id :
                wrong_seq +=1
                #console.log(f"Not sequential seq ID  {wf.get_daqheader().seq_id} for stream ID {wf.get_daqheader().stream_id}! Previous sequence ID was {seq_checker[wf.get_daqheader().stream_id%4]}.")
            seq_checker[wf.get_daqheader().stream_id%4] = wf.get_daqheader().seq_id
            rcv_pkts_tot +=1
            rcv_pkts +=1
            if rcv_pkts_tot % 100000 == 0:
                stop_time = time.time()
                interval = stop_time - start_time
                console.log(f"Received {rcv_pkts} packets: throughput is {rcv_bytes*8/(interval*1000000000.):.2f} Gb/s")
                rcv_pkts = 0
                rcv_bytes = 0
                start_time = stop_time
        except KeyboardInterrupt:
            break
        except:
            continue
    console.log(f"Received {rcv_pkts_tot} messages; ending now.")

if __name__ == '__main__':
    try:
        cli(show_default=True, standalone_mode=True)
    except Exception as e:
        console.print_exception()
