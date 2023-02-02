# system-on-a-business-card (SoBC)

The System on a Business Card is a miniature system in the size of a business card.
It is controlled by a ESP32-C3 microncontroller which shall run Zephyr RTOS in the future.
Since this goal is to also use the PCB is a business card, the predominant factor is the size which is 55x85 mm which seems to be the standard at least for Europe.

# Hardware Design

The system uses the on-board USB Type C recepticle for power delivery and converts the 5V to 3.3V using an LDO.
Since the ESP32-C3 has a USB interface and provides a serial interface as well as a JTAG no additional UART connector is on the board.
As the microcontroller also supports the connection of an antenne, there is also a PCB inverted F antenna on the top left of the board.
One of the most prominent elements of the board is 0.77 inch OLED screen which is connected via I2C to the host and is powered via the USB's 5V.
Instead of using typical low profile buttons, like on the top for BOOT and RESET, the board has resistive touch pads. 
As a small added benefit it also reduces costs.

# Custimisation
As this is a business card, it shall display your contact information.
Therefore, the bottom silkscreen layer provides a text field for this.
In addition, below the text field I added a QR-Code which was generated using [this online generator](https://goqr.me/#t=vcard). 

To modify this open the PCB design view of KiCAD and best hide all layers except Edge.Cuts and B.Silkscreen.
On the right side you find the text box for your contact information and below there is some space to place the QR-Code.
The QR-Code needs to be available as svg graphic and can be imported via File->Import->Graphics.
If you use the same generator, you can import the svg with a scaling factor of 0.375.

## Future Improvements to the Design

### Applications
At the moment I'm working on getting Zephyr running and haven't really decided what to actually run on it.
If you have an idea feel free to start a discussion in the discussions tab.

### ESP32-C6
Esprressif has announced the ESP32-C6 with a second conre and more wireless connectivity standards.
At the moment the chip is not available on my default parts provider (LCSC), but it would be nice to check it out due to its more powerfull CPU.
Further investigation is strongly dependent on the price.

### USB Ciruitry
Currently the design simply pulls power through the USB plug, but this violates the USB specification. 
For future revisions it would be awesome to add a USB power negotion circuit to board to add comformoty. 
If you're familiar with the design of such circuits feel free to add some comments.
