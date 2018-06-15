Memory map
========================

## Low memory

<table>
  <tr>
    <th>Offset<br></th>
    <th>End (-1 byte [1])</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>`0x00000000`<br></td>
    <td>`0x00000500`</td>
    <td>IVT reserved</td>
  </tr>
  <tr>
    <td>`0x00000500`<br></td>
    <td>`0x00005500`</td>
    <td>FAT12 and root directory</td>
  </tr>
  <tr>
    <td>`0x00005500`</td>
    <td>`0x00007C00`</td>
    <td>Reserved for system stack<br></td>
  </tr>
  <tr>
    <td>`0x00007C00`</td>
    <td>`0x00010000`</td>
    <td>* Bootsector<br>* Bootloader (stage 2)<br>* Devices information block (Max devices 1408)<br></td>
  </tr>
  <tr>
    <td>`0x00010000`<br></td>
    <td>`0x00050000`<br></td>
    <td>Reserved for kernel image<br></td>
  </tr>
  <tr>
    <td>`0x00050000`</td>
    <td>`0x00050080`</td>
    <td>Initializers</td>
  </tr>
  <tr>
    <td>`0x0009FC00`<br></td>
    <td>`0x000A0000`</td>
    <td>EBDA BIOS reserved<br></td>
  </tr>
  <tr>
    <td>`0x000A8000`</td>
    <td>`0x00100000`</td>
    <td>Video memory</td>
  </tr>
</table>
[1] - Decrement each segment end with 1 to get actual desired segment.
This is a requirement for vanity of this memory map overall.

## High memory

All physical memory starting from `0x00100000` is managed by the memory manager.

Last edited for `build_2`.

**Luka Andjelkovic**
