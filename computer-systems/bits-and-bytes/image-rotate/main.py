from __future__ import annotations

import struct
from array import array
from abc import ABC, abstractmethod


class Serializable(ABC):
    @classmethod
    @abstractmethod
    def from_bytes(cls, b: bytes) -> Serializable:
        """Serialize from bytes"""
        raise NotImplementedError


class BmpFile(Serializable):
    """Simplified object representation of a BMP file"""

    def __init__(
        self,
        file_header: BmpFile.FileHeader,
        dib_header: BmpFile.DibHeader,
        pixel_array: BmpFile.PixelArray,
    ) -> None:
        self._file_header = file_header
        self._dib_header = dib_header
        self._pixel_array = pixel_array

    def rotate(self) -> None:
        """Mutates this instance's pixel array by rotating 90deg"""
        cols = self.dib_header.width
        rows = self.dib_header.height
        arr = self.pixel_array.array

        # NOTE: a slightly different (and less readable) take on Oz's equation
        rotated: list[array] = [
            arr[j * rows + (rows - i - 1)] for i in range(rows) for j in range(cols)
        ]

        self.pixel_array.set_array(rotated)

    @classmethod
    def from_bytes(cls, b: bytes) -> BmpFile:
        file_header = BmpFile.FileHeader.from_bytes(b[:14])
        dib_header = BmpFile.DibHeader.from_bytes(b[14:54])
        pixel_array = BmpFile.PixelArray.from_bytes(
            b[file_header.offset : file_header.offset + dib_header.size]
        )
        assert (
            len(pixel_array.array) == dib_header.width * dib_header.height
        ), "File is curropted"
        return cls(file_header, dib_header, pixel_array)

    @property
    def file_header(self) -> BmpFile.FileHeader:
        return self._file_header

    @property
    def dib_header(self) -> BmpFile.DibHeader:
        return self._dib_header

    @property
    def pixel_array(self) -> BmpFile.PixelArray:
        return self._pixel_array

    class FileHeader(Serializable):
        def __init__(self, identity: str, file_size: int, offset: int) -> None:
            self._identity = identity
            self._file_size = file_size
            self._offset = offset

        @classmethod
        def from_bytes(cls, b: bytes) -> BmpFile.FileHeader:
            assert len(b) == 14, "Invalid size for file header"

            identity = b[:2].decode("ascii")
            file_size = struct.unpack("<I", b[2:6])[0]
            offset = struct.unpack("<I", b[10:])[0]

            return cls(identity, file_size, offset)

        @property
        def identity(self) -> str:
            return self._identity

        @property
        def file_size(self) -> int:
            return self._file_size

        @property
        def offset(self) -> int:
            return self._offset

        def __repr__(self) -> str:
            return f"ID={self.identity},FILE_SIZE={self.file_size},OFFSET={self.offset}"

    class DibHeader(Serializable):
        def __init__(self, width: int, height: int, size: int) -> None:
            self._width = width
            self._heigt = height
            self._size = size

        @classmethod
        def from_bytes(cls, b: bytes) -> BmpFile.DibHeader:
            assert len(b) == 40, "Invalid size for DIB header"

            # NOTE: width and height are intentionally signed
            width = struct.unpack("<i", b[4:8])[0]
            height = struct.unpack("<i", b[8:12])[0]
            size = struct.unpack("<I", b[20:24])[0]

            return cls(width, height, size)

        @property
        def width(self) -> int:
            return self._width

        @property
        def height(self) -> int:
            return self._heigt

        @property
        def size(self) -> int:
            return self._size

        def __repr__(self) -> str:
            return f"WIDTH={self.width},HEIGHT={self.height}"

    class PixelArray(Serializable):
        def __init__(self, arr: list[array]) -> None:
            self._arr = arr

        @classmethod
        def from_bytes(cls, b: bytes) -> BmpFile.PixelArray:
            arr = [array("B", b[n : n + 3]) for n in range(0, len(b), 3)]
            return cls(arr)

        @property
        def array(self) -> list[array]:
            return self._arr

        def __eq__(self, other: BmpFile.PixelArray) -> bool:
            return self.array == other.array

        def set_array(self, arr: list[array]) -> None:
            self._arr = arr


if __name__ == "__main__":
    test_cases = [
        ("teapot.bmp", "teapot-rotated.bmp"),
    ]

    for in_file, out_file in test_cases:
        with open(in_file, "rb") as stream:
            source = BmpFile.from_bytes(stream.read())

        with open(out_file, "rb") as stream:
            target = BmpFile.from_bytes(stream.read())

        source.rotate()

        assert source.pixel_array == target.pixel_array
