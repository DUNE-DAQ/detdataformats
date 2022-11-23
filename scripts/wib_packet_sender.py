#!/usr/bin/env python3
import socket
import time
import math
import click
from rich.console import Console
import detdataformats

console = Console()

CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])
@click.command(context_settings=CONTEXT_SETTINGS)
@click.option('-a','--address', default='127.0.0.1', help='Destination IP address')
@click.option('-p','--port', default=1234, help='Port number')


def cli(address, port):
    console.log(f"Preparing to send WIB packets to {address}:{port} in a loop.")
    sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

    wf = detdataformats.wibeth.WIBEthFrame()
    wf.get_daqheader().det_id       = 3
    wf.get_daqheader().stream_id    = 0
    wf.get_daqheader().seq_id       = 1
    wf.get_daqheader().block_length = 0x382
    wf.get_daqheader().timestamp    = 0

    sent_pkts = 0
    sent_pkts_tot = 0
    sent_bytes = 0
    sent_bytes_tot = 0
    start_time_orig = time.time();
    start_time = time.time();
    while True:
        try:
            for i in range(0,4):
                wf.get_daqheader().stream_id    = i
                sock.sendto(wf.get_bytes(), (address, port))
                sent_pkts +=1
                sent_pkts_tot +=1
                sent_bytes += len(wf.get_bytes())
                sent_bytes_tot += len(wf.get_bytes())
                if sent_pkts_tot % 100000 == 0:
                    stop_time = time.time()
                    interval = stop_time - start_time
                    console.log(f"Received {sent_pkts} packets: throughput is {sent_bytes*8/(interval*1000000000.):.2f} Gb/s")
                    sent_pkts = 0
                    sent_bytes = 0
                    start_time = stop_time

            wf.get_daqheader().timestamp += (32*64)
            if wf.get_daqheader().seq_id == 4095:
                wf.get_daqheader().seq_id = 0
            else:    
                wf.get_daqheader().seq_id +=1

        except KeyboardInterrupt:
            break
        except:
            continue
    
    stop_time = time.time();
    time_seconds = stop_time - start_time_orig
    console.log(f"Sent {sent_pkts_tot} WIB packets in {time_seconds:.2f} seconds. Throughput = {(sent_bytes_tot*8)/(time_seconds*1000000000.):2f} Gb/s")

if __name__ == '__main__':
    try:
        cli(show_default=True, standalone_mode=True)
    except Exception as e:
        console.print_exception()

