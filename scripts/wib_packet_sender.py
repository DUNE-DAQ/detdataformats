import socket
import time
import click
from rich.console import Console

console = Console()

CONTEXT_SETTINGS = dict(help_option_names=['-h', '--help'])
@click.command(context_settings=CONTEXT_SETTINGS)
@click.option('-a','--address', default='127.0.0.1', help='Destination IP address')
@click.option('-p','--port', default=1234, help='Port number')
@click.option('-t', '--time-seconds', default=5, help='How many seconds long to send WIB packets')


def cli(address, port, time):
    console.log(f"Preparing to send WIB packets to {address}:{port} for {time} seconds")
    sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
    sent_pkts = 0
    timeout = time.time() + time_sec
    while time.time() < timeout:
        sock.sendto(wibpacket, (address, port))
        sent_pkts +=1

    console.log(f"Sent {sent_pkts} WIB packets")

if __name__ == '__main__':
    try:
        cli(show_default=True, standalone_mode=True)
    except Exception as e:
        console.print_exception()




sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
