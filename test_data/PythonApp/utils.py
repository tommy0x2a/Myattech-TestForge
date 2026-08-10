"""
utils.py

Realistic Python utility module for an embedded systems test harness.
Provides checksum calculation, register map parsing, and serial protocol handling.
"""

import struct
from pathlib import Path
from typing import Any, Dict, Optional

try:
    import serial  # type: ignore
except ImportError:
    serial = None  # type: ignore


def calculate_checksum(data: bytes) -> int:
    """Simple XOR checksum for embedded protocol frames."""
    checksum = 0
    for byte in data:
        checksum ^= byte
    return checksum & 0xFF


def parse_register_map(filepath: str) -> dict[str, int]:
    """Parse a simple register map file (name = address)."""
    reg_map: dict[str, int] = {}
    path = Path(filepath)
    if not path.exists():
        return reg_map

    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if "=" in line:
                name, addr = line.split("=", 1)
                reg_map[name.strip()] = int(addr.strip(), 0)
    return reg_map


def format_hex(value: int, width: int = 8) -> str:
    """Format an integer as a zero-padded hex string."""
    return f"0x{value:0{width}X}"


class SerialProtocol:
    """Simple serial protocol handler for talking to embedded devices."""

    def __init__(self, port: str, baudrate: int = 115200):
        self.port = port
        self.baudrate = baudrate
        self._ser: Optional[Any] = None

    def connect(self) -> bool:
        if serial is None:
            return False
        try:
            self._ser = serial.Serial(self.port, self.baudrate, timeout=1.0)
            return self._ser.is_open
        except Exception:
            return False

    def send_command(self, cmd: bytes, timeout: float = 1.0) -> bytes:
        if not self._ser or not self._ser.is_open:
            return b""
        self._ser.write(cmd)
        self._ser.timeout = timeout
        response = self._ser.read(64)
        if self._validate_response(response):
            return response
        return b""

    def disconnect(self) -> None:
        if self._ser and self._ser.is_open:
            self._ser.close()
        self._ser = None

    def _validate_response(self, response: bytes) -> bool:
        """Private method: basic length and checksum validation."""
        if len(response) < 2:
            return False
        return calculate_checksum(response[:-1]) == response[-1]


class RegisterMap:
    """In-memory register map abstraction."""

    def __init__(self, base_address: int):
        self.base_address = base_address
        self._registers: Dict[int, int] = {}

    def read(self, offset: int) -> int:
        addr = self.base_address + offset
        return self._registers.get(addr, 0)

    def write(self, offset: int, value: int) -> None:
        addr = self.base_address + offset
        self._registers[addr] = value & 0xFFFF
