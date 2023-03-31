# Hardware Design

The system uses the on-board USB Type C recepticle for power delivery and converts the 5V to 3.3V using an LDO.
Since the ESP32-C3 has a USB interface and provides a serial interface as well as a JTAG no additional UART connector is on the board.
As the microcontroller also supports the connection of an antenne, there is also a PCB inverted F antenna on the top left of the board.
One of the most prominent elements of the board is 0.77 inch OLED screen which is connected via I2C to the host and is powered via the USB's 5V.
Instead of using typical low profile buttons, like on the top for BOOT and RESET, the board has resistive touch pads. 
As a small added benefit this design decision also reduces costs.

Since this PCB was designed to be also used as a business card, we opted for a thickness of 0.8mm. 
Main reason for the selected thickness, is that JLCPCB does not charge extra for PCB's between 0.8 and 1.6 mm.
This has to be taken with a grain of salt, other may charge extra for this.
The design for the antenna would be simpler to match the impedance, if a 4-Layer would be used. 
Unfortunately, with the thinner PCB significant extra costs would be added when using more layers.

## Board Revision History

A board revisions looks as follows ``MAJOR.MINOR.PATCH``.
``MAJOR`` revisions indicate some fundamental changes in the design, e.g, a new host. 
``MINOR`` revisions indicate changes in the hardware design, which requires a change in e.g. the Zephyr device tree.
Different ``PATCH`` versions indicate some minor changes in the design, which does not affect the software side.

| Revision | Local Link | Github Link | Release Package | Comments |
|---|---|---|---|---|
| 1.0.0/1.A  | [BOM](../system-on-a-business-card/bom/ibom_rev_1_0_0.html) | [BOM](https://htmlpreview.github.io/?https://github.com/epsilon-0311/system-on-a-business-card/blob/main/KiCAD/system-on-a-business-card/bom/ibom_rev_1_0_0.html)| [Package](https://github.com/epsilon-0311/system-on-a-business-card/releases/tag/board_rev_1_0_0) | Initial Version |
| 1.0.1 | [BOM](../system-on-a-business-card/bom/ibom_rev_1_0_1.html) | [BOM](https://htmlpreview.github.io/?https://github.com/epsilon-0311/system-on-a-business-card/blob/main/KiCAD/system-on-a-business-card/bom/ibom_rev_1_0_1.html)| [Package](https://github.com/epsilon-0311/system-on-a-business-card/releases/tag/board_rev_1_0_1) | <ul><li>Fixed unconnected OLED Logic Supply Voltage</li><li>Updated Resistor to dimm LED D1</li><li>Fixed Flash  IC Footprint</li></ul> |

The first batch was produced using the revision 1.A.
In order to provide a revision compatible with Zephyr, it was changed to ``1.0.0``.

## Antenna Impedance Trace Width Matching

As this design utilises a PCB Antenna, impedance matched traces are required.
The calculations assummed a surface micrstrip design.
From the manufacturer JLCPCB we know a copper thickness of 1 oz/35µm(see [Finished Outer Layer Copper](https://jlcpcb.com/capabilities/pcb-capabilities)) and a dilectric contant of 4.5 (see [Dielectric constant](https://jlcpcb.com/capabilities/pcb-capabilities)).
Using this [calculator](https://www.pcbway.com/pcb_prototype/impedance_calculator.html) we calculated a required track width of 1.45 mm to achieve a impedance of 50 Ohm.
Due to space limiations we opted for a trace width of 1.2 mm.

# Customization of the PCB
As this is a business card, it shall display your contact information.
Therefore, the bottom silkscreen layer provides a text field for this.
In addition, below the text field I added a QR-Code which was generated using [this online generator](https://goqr.me/#t=vcard). 

To modify this open the PCB design view of KiCAD and best hide all layers except Edge.Cuts and B.Silkscreen.
On the right side you find the text box for your contact information and below there is some space to place the QR-Code.
The QR-Code needs to be available as svg graphic and can be imported via File->Import->Graphics.
If you use the same generator, you can import the svg with a scaling factor of 0.375.

# Sourcing Guide 

You can find a sourcing guide using [JLCPCB](https://jlcpcb.com/) and [LCSC](https://www.lcsc.com/) as PCB manufacturer and parts suppliere here: [Link](https://docs.google.com/spreadsheets/d/16gFrFvvIfR94LbLsq9TxAxXNphaDNR22jH06x7NUitE/edit?usp=sharing). 
With the components selected in this Google Spreads, a single PCB with parts will cost around 4€, if at least 10 are manufactured.
Due to varying costs on the shipment destination, shipment costs were not included in the calculation.

The original goal was to be at around 3€ per board.
The main cost factors are the used microcontroller (ESP32-C3) and the 0.77 inch OLED display.
If you are aware of components that can reduce the price, feel free to discuss it in the discussion section on GitHub. 

# Possible Future updates to the Design

## ESP32-C6
Esprressif has announced the ESP32-C6 with a high and a lowe power core and more wireless connectivity standards.
At the moment the chip is not available on my default parts provider (LCSC), but it would be nice to check it out due to its more powerfull CPU.
Further investigation is strongly dependent on the price.

## USB Ciruitry
Currently the design simply pulls power through the USB plug, but this violates the USB specification. 
For future revisions it would be awesome to add a USB power negotion circuit to board to add comformoty. 
If you're familiar with the design of such circuits feel free to add some comments.
