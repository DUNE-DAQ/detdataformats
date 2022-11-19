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

    rcv_pkts = 0
    rcv_size = 0
    while True:      
        wf = detdataformats.wibeth.WIBEthFrame()
        data, addr = sock.recvfrom(8192)
        wf = detdataformats.wibeth.WIBEthFrame(data)
        console.log(f"Received packet from {addr} with sequence ID  {wf.get_daqheader().seq_id}")
        rcv_pkts +=1



if __name__ == '__main__':
    try:
        cli(show_default=True, standalone_mode=True)
    except Exception as e:
        console.print_exception()
