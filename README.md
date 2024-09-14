FITS file superposition, calibration, denoising and alignment software is a software for processing FITS files, which can be used to superimpose, calibrate, denoise, align and demosaic FITS files. The software is developed using QT, and the interface is simple and easy to use.

**Features:**

* Superposition of FITS files: Multiple FITS files can be superimposed to generate a new FITS file.
* Calibration of FITS files: FITS files can be calibrated to eliminate system errors.
* Denoising of FITS files: FITS files can be denoised to improve image quality.
* Alignment of FITS files: FITS files can be aligned to eliminate image distortion.
* Debayer sequence of FITS files: The software can perform demosaicing operations on FITS files to convert the original Bayer format images to RGB format images.

**How to use:**

1. Open the software and select the "Open File" button to select the FITS file to be processed.
2. Select the operation to be performed.
3. Click the "Start" button to start the processing.

**Precautions:**

* The software requires you to download the OpenCV library yourself and place the opencv_world480d.dll file in the bin folder under opencv and under debug.
* The software uses the cfitsio library to read and write FITS files.
* The software uses the OpenCV library for image processing, including alignment, noise reduction and so on.
* The software uses the Qt library to build the graphical user interface.



