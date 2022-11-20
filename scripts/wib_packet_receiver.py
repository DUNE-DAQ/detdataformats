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
        try:
            data, addr = sock.recvfrom(8192)
            wf = detdataformats.wibeth.WIBEthFrame(data)
            #console.log(f"Received packet from {addr} with stream ID {wf.get_daqheader().stream_id},  sequence ID  {wf.get_daqheader().seq_id}, time {wf.get_daqheader().timestamp}")
            rcv_pkts +=1
        except KeyboardInterrupt:
            break
        except:
            continue
    console.log(f"Received {rcv_pkts} messages; ending now.")

if __name__ == '__main__':
    try:
        cli(show_default=True, standalone_mode=True)
    except Exception as e:
        console.print_exception()