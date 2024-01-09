
简介

FITS文件叠加、校准、降噪、对齐软件是一款用于FITS文件处理的软件，可对FITS文件进行叠加、校准、降噪和对齐等操作。软件使用QT开发，界面简洁易用。目前软件已完成对齐功能，部分降噪功能。

软件需要自行下载OpenCV库，将opencv_world480d.dll放在opencv下bin中即可。

软件使用了以下库：

cfitsio：用于读取和写入FITS文件的库。

OpenCV：用于图像处理的库。

Qt：用于图形用户界面的库。

FITS file superposition, calibration, denoising and alignment software is a software for processing FITS files, which can be used to superimpose, calibrate, denoise and align FITS files. The software is developed using QT, and the interface is simple and easy to use. The software has currently completed the alignment function, and some denoising functions.

The software requires you to download the OpenCV library yourself and place the opencv_world480d.dll file in the bin folder under opencv.

The software uses the following libraries:

cfitsio: A library for reading and writing FITS files

OpenCV: A library for image processing.

Qt: A library for graphical user interfaces.

功能：

FITS文件叠加：可将多个FITS文件叠加在一起，生成一个新的FITS文件。

FITS文件校准：可对FITS文件进行校准，消除系统误差。

FITS文件降噪：可对FITS文件进行降噪，提高图像质量。

FITS文件对齐：可对FITS文件进行对齐，消除图像畸变。

Superposition of FITS files: Multiple FITS files can be superimposed to generate a new FITS file.

Calibration of FITS files: FITS files can be calibrated to eliminate system errors.

Denoising of FITS files: FITS files can be denoised to improve image quality.

Alignment of FITS files: FITS files can be aligned to eliminate image distortion.

使用方法

软件使用方法如下：

打开软件，选择“打开文件”按钮，选择要处理的FITS文件。

选择要进行的操作。

点击“开始”按钮，软件将开始处理。

The usage of the software is as follows:

Open the software and select the "Open File" button to select the FITS file to be processed.
Select the operation to be performed.
Click the "Start" button to start the processing.

展望

未来，软件将继续开发，增加更多功能，包括：

支持更多类型的FITS文件。

支持更多校准和降噪算法。

支持更高级的对齐算法。

In the future, the software will continue to be developed to add more features, including:

Support for more types of FITS files.
Support for more calibration and denoising algorithms.
Support for more advanced alignment algorithms.
注意事项

软件需要自行下载OpenCV库，将opencv_world480d.dll放在opencv下bin中与debug下即可。

软件使用了cfitsio库来读取和写入FITS文件。

软件使用了OpenCV库来进行图像处理，包括对齐、降噪等。

软件使用了Qt库来构建图形用户界面。
