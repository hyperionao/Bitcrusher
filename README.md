# **Bitcrusher Audio Processing Tool**

## **Overview**
The Bitcrusher Audio Processing Tool is a high-performance library written in **C++/C** to apply **bitcrushing** and **sample rate reduction** effects to audio files. It uses **libsndfile** for audio input/output and incorporates custom low-pass filtering and chunk-based processing for large audio files.

---

## **Features**
- **Bitcrushing Effects**:
  - Apply bit-depth reduction to create vintage or distorted audio.
- **Sample Rate Reduction**:
  - Reduce the sample rate for creative sound design.
- **Low-Pass Filtering**:
  - Smooth transitions and aliasing prevention using a custom FIR filter.
- **Chunk-Based Processing**:
  - Memory-efficient handling of large audio files.
- **Error Handling**:
  - Robust exception handling for file I/O and parameter validation.

---

## **Getting Started**

### **Requirements**
- **C++ Compiler**: GCC (MinGW recommended for Windows) with support for C++17.
- **Library**: [libsndfile](http://www.mega-nerd.com/libsndfile/).

---

### **Building and Running**

#### **1. Building with g++**
Use the following command to compile the project:
```bash
g++ -g -o test_bitcrusher src/main.cpp src/Bitcrusher.cpp src/filemanager.cpp \
src/filtermanager.cpp src/FIRFilter.cpp src/processaudiochunks.cpp src/lowpasscoefs.cpp \
src/audioprocessor.cpp src/quantizer.cpp -Iinclude -lsndfile
```

#### **2. Running the Executable**
After building, run the tool with the following command:
```bash
./test_bitcrusher <input_file> <output_file> <bit_depth> <sample_rate_reduction_factor>
```

#### **Example**
```bash
./test_bitcrusher 
```

- You need to adjust main.cpp to use this program. Update input and output path. Experiment by changing sample rate reduction factor and bit rate. 

---

## **Project Structure**
```plaintext
Bitcrusher/
├── include/               # Header files for modular design
│   ├── audioprocessor.h   # Audio processing logic
│   ├── Bitcrusher.h       # Facade class
│   ├── filemanager.h      # File handling utilities
│   ├── filtermanager.h    # Filtering logic
│   ├── FIRFilter.h        # FIR filter implementation
│   ├── lowpasscoefs.h     # Low-pass filter coefficient calculation
│   ├── processaudiochunks.h # Chunk-based processing logic
│   ├── quantizer.h        # Audio quantization logic
├── src/                   # Source files
│   ├── audioprocessor.cpp
│   ├── Bitcrusher.cpp
│   ├── filemanager.cpp
│   ├── filtermanager.cpp
│   ├── FIRFilter.cpp
│   ├── lowpasscoefs.cpp
│   ├── processaudiochunks.cpp
│   ├── quantizer.cpp
│   ├── main.cpp           # Entry point for testing
├── test.wav               # Sample input audio file
├── README.md              # Project documentation
```

---

## **Usage**

### **1. Input File**
- Ensure your `test.wav` file is located in the project directory.

### **2. Running the Program**
- Open a terminal and navigate to the project directory.
- Run the compiled executable:
  ```bash
  ./test_bitcrusher
  ```
- Adjust the `bit_depth` and `sample_rate_reduction_factor` as needed to achieve the desired audio effects.

---

## **Future Enhancements**
- Extend functionality for multi-threaded processing.

---

## **Contributing**
Contributions are welcome! If you have suggestions or improvements, feel free to open issues or submit pull requests.

---

## **License**
This project is licensed under the MIT License.
