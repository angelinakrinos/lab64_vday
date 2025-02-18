# **Lab64 Valentine's Day Music Box 🎵❤️**  
An Arduino-controlled music player for a Lab64 workshop, using an **ESP32C6**, **DFPlayer Mini**, and **RFID module** to create a customizable music box.

## **Workshop Instructions**  

### **Hardware Setup**  
#### 🔧 **Soldering Your Board**  
1. **Microcontroller (XIAO ESP32C6)**  
   - The **battery pads** on the back should be soldered first.  
   - Apply a small amount of solder to the pads before attaching the leads.  
   - Ensure the leads are inserted into the **thru holes** marked **BAT- and BAT+**
   (the leads will need to cross each other; **double-check polarity!**).  
   - Solder header pins onto the microcontroller.  

2. **RFID Module**  
   - Solder **longer header pins** into your RFID module for better connection.  

3. **Soldering Components Onto the Board**  
   - **Terminal blocks** should face **outward** so wires can be easily inserted.  
   - The **battery holder** has clearly marked **positive and negative terminals**—pay attention to **snap-in orientation**.  
   - **RFID module** should be soldered as high as possible for better range.  

4. **Connecting Wires**  
   - Terminal blocks will connect to wires, which will connect to your peripherals (speaker, potentiometer, buttons, etc.).  

---

### **Software Setup**  
#### 💻 **Preparing Your Development Environment**  
1. Install **Arduino IDE** (if you haven’t already).
2. Install the **XIAO ESP32C6 board configuration** in the Arduino IDE (if you don't already have it),
   following [these instructions](https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/#software-preparation)
3. Install the following two code dependencies.
   In the Arduino IDE, open the library manager
   (book icon in left sidebar, or `Ctrl/Cmd-Shift-I`)
   and search for the appropriate library.
   To make sure you have the right search result,
   the "More Info" item in the options menu
   next to the search result should take you to the same place linked here.
   1. [`RFID_MFRC522v2`](https://github.com/OSSLibraries/Arduino_MFRC522v2),
      for communicating with the RFID reader.
   2. [`DFPlayerMini_Fast`](https://github.com/PowerBroker2/DFPlayerMini_Fast)
      for communicating with the mp3 player.

#### 📀 **Writing RFID Tags & Playing Music**  
1. **RFID Writing**  
   - Use the provided `rfid_writer.ino` code to write RFID tags.
   - Open the file in the Arduino IDE,
     select the correct **board (ESP32C6)** and **port**,
     then upload the code (➡️ icon in the upper menu bar).
   - In the Arduino IDE, open up the serial monitor
     (`Tools > Serial Monitor`, or `Ctrl/Cmd-Shift-M`)
   - Scan your RFID cards on the RFID card (`RFID-RC522`) one at a time.
     For each card, the microcontroller will emit a message like the following
     when it succeeds.
     ```
     Card UID:  D8 CC 69 B9
     Data written successfully in block: 2
     Verification read successful!
     Data in block 2: Track 0001
     ```
     The writer will **batch-write** tags **sequentially from 1 → ∞**
     for each card you scan.

2. **Music File Setup**  
   - Format your **microSD card to FAT32**, if it isn't already
   - Inside the SD card, create a folder named **`mp3`**.  
   - Add your MP3 files inside the folder, naming them sequentially:  
     ```
     0001.mp3  
     0002.mp3  
     0003.mp3  
     ...
     ```
   - Insert the **SD card into the DFPlayer Mini**
   
3. **Uploading the Code** 
   - You need to **push the `lab64_vday.ino` code** to your **ESP32C6**
     for the music box to function.
   - **swipe an RFID tag** to play its corresponding track.  

---

### **Building the Music Box**  
#### 📦 **Assembling the Enclosure**  
1. Gather all the **box panels**.  
2. **Glue** the sides together using **wood glue**.  
3. **Before attaching the front panel**, install the **back panel** with fasteners—this is where peripherals like:  
   - 🎵 **Speaker**  
   - 🎛️ **Potentiometer**  
   - 🔘 **Buttons**  
   will be mounted.  
4. Customize the **lid** and print it to complete the box!  


🚀 **Enjoy your Valentine's Day Music Box!** 🎶❤️  
