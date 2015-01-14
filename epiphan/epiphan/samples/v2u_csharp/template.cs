//--------------------------------------------------------
// Imports
//
[DllImport("kernel32.dll", SetLastError = true)]
static extern IntPtr CreateFile(
	string lpFileName,
	uint dwDesiredAccess,
	uint dwShareMode,
	IntPtr lpSecurityAttributes,
	uint dwCreationDisposition,
	uint dwFlagsAndAttributes,
	IntPtr hTemplateFile);

[DllImport("kernel32.dll", SetLastError = true)]
static extern int CloseHandle(IntPtr hObject);

[DllImport("kernel32.dll", SetLastError = true)]
static extern bool DeviceIoControl(
	IntPtr hDevice,
	uint dwIoControlCode,
	IntPtr lpInBuffer,
	uint nInBufferSize,
	[Out] IntPtr lpOutBuffer,
	uint nOutBufferSize,
	ref uint lpBytesReturned,
	IntPtr lpOverlapped);

//--------------------------------------------------------
// Definitions
//
private const uint GENERIC_READ = 0x80000000;
private const uint GENERIC_WRITE = 0x40000000;
private const int OPEN_EXISTING = 3;
private const int INVALID_HANDLE_VALUE = -1;
private const int IOCTL_GRABFRAME_CODE = 0x0022002B;
private const int V2U_MAX_W = 1600;
private const int V2U_MAX_H = 1200;
private const int V2U_BPP = 3;

internal struct IOCTL_GRABFRAME
{
    public IntPtr pixbuf; /* IN should be filled by user process */
    public int pixbuflen; /* IN should be filled by user process */
    public int width; /* OUT width in pixels */
    public int height; /* OUT height in pixels */
    public int bpp; /* IN/OUT bits per pixel (16 or 24) */
} 



//--------------------------------------------------------
// STEP 1. Open the driver
//
IntPtr driverHandle = new IntPtr(); 
for(int i=0;i<16;i++) {
    driverHandle = CreateFile(@"\\.\EpiphanVga2usb" + i.ToString(), GENERIC_READ|GENERIC_WRITE , 0 , IntPtr.Zero, OPEN_EXISTING, 0, IntPtr.Zero );
    if((int)driverHandle != INVALID_HANDLE_VALUE) {
        break;
    }
}



//--------------------------------------------------------
// STEP 2. Prepare data structure to receive pixels
//
IntPtr framePointer;
IOCTL_GRABFRAME frame;
frame = new IOCTL_GRABFRAME();
frame.pixbuflen = V2U_MAX_W * V2U_MAX_H * V2U_BPP;
frame.pixbuf = Marshal.AllocHGlobal(frame.pixbuflen);
frame.bpp = 24;
uint size = Marshal.SizeOf(frame);
unsafe
{
    framePointer = new IntPtr(&frame);
}



//--------------------------------------------------------
// STEP 3. Receive pixels and image resolution to the data structure
//
uint retsize;
DeviceIoControl(driverHandle, IOCTL_GRABFRAME_CODE, framePointer, size, framePointer, size, ref retsize, IntPtr.Zero);



//--------------------------------------------------------
// STEP 4. Receive pixels to the managed byte array
// You can skip it if you want to keep pixels in unmanaged memory
//
Byte[] bytes = new Byte[frame.pixbuflen];
Marshal.Copy(frame.pixbuf, bytes, 0, bytes.Length);



//--------------------------------------------------------
// STEP 5. Do something with pixels...
// For example we will import pixels into Bitmap object
//
// Get pointer to the bitmap pixel array
Bitmap bitmap = new Bitmap(frame.width, frame.height);
int size = frame.width * frame.height * 3;
BitmapData bmpData = bitmap.LockBits(new Rectangle(0, 0, frame.width, frame.height), ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb);
IntPtr intPtr = bmpData.Scan0;

// Copy the RGB values back to the bitmap
// Works fine if (frame.width % 4)==0 which is true for practically all VGA modes, otherwise do line-by-line copy
Marshal.Copy(bytes, 0, intPtr, size);

// Unlock the bits.
bitmap.UnlockBits(bmpData);

// Try Bitmap.Save(file,format) here
....



//--------------------------------------------------------
// STEP 6. Close the device
// Don't forget to clean other resources like frame.pixbuf if you are not going to reuse them
//
CloseHandle(driverHandle);

