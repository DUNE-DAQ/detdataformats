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
@click.option('-t', '--time-seconds', default=5, help='How many seconds long to send WIB packets')


def cli(address, port, time_seconds):
    console.log(f"Preparing to send WIB packets to {address}:{port} for {time_seconds} seconds")
    sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

    wf = detdataformats.wibeth.WIBEthFrame()
    wf.get_daqheader().det_id       = 3
    wf.get_daqheader().stream_id    = 0
    wf.get_daqheader().seq_id       = 0
    wf.get_daqheader().block_length = 0x382
    wf.get_daqheader().timestamp    = 0

    sent_pkts = 0
    timeout = time.time() + time_seconds
    while time.time() < timeout:        
        sock.sendto(wf.get_bytes(), (address, port))
        sent_pkts +=1
        wf.get_daqheader().timestamp += (32*64)
        wf.get_daqheader().seq_id +=1
        time.sleep(1)
    console.log(f"Sent {sent_pkts} WIB packets")

if __name__ == '__main__':
    try:
        cli(show_default=True, standalone_mode=True)
    except Exception as e:
        console.print_exception()

