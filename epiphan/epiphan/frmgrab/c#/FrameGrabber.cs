//
// $Id: FrameGrabber.cs 13356 2011-06-16 21:52:28Z monich $
//
// Copyright (C) 2008-2011 Epiphan Systems Inc. All rights reserved.
//
// C# wrapper for frmgrab.dll
//

using System;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;

namespace Epiphan.FrmGrab
{
    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct V2USize
    {
        [FieldOffset(0)]  public int width;
        [FieldOffset(4)]  public int height;
    }

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct V2URect
    {
        [FieldOffset(0)]  public int x;
        [FieldOffset(4)]  public int y;
        [FieldOffset(8)]  public int width;
        [FieldOffset(12)] public int height;
    }

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct V2U_VideoMode
    {
        [FieldOffset(0)]  public int width;   // screen width, pixels
        [FieldOffset(4)]  public int height;  // screen height, pixels
        [FieldOffset(8)]  public int vfreq;   // vertical refresh rate, mHz
    }

    // Well, what about 32 vs 64 bits?
    public unsafe struct V2U_GrabFrame2 {
        public void* pixbuf;
        public int pixbuflen;
        public uint palette;
        public V2URect crop;
        public V2U_VideoMode mode;
        public int imagelen;
        public int retcode;
    }

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct V2U_GrabParameters
    {
        // Validity flags for fields below. When any of the fields below
        // is used, the corresponding V2U_FLAG_VALID_* flag  is set.
        // Otherwise the field is ignored.
        [FieldOffset(0)]  public uint flags;
        // Shifts image left (<0) or right(>0). Valid range depends on
        // the current video mode and the hardware capabilities.
        // Invalid values are rounded to the nearest valid value.
        [FieldOffset(4)]  public int hshift;
        // Pixel sampling phase, [0,31]
        [FieldOffset(8)]  public byte phase;
        // Gain (contrast) for the red channel, [0,255]
        [FieldOffset(9)]  public byte gain_r;
        // Gain (contrast) for the green channel, [0,255]
        [FieldOffset(10)] public byte gain_g;
        // Gain (contrast) for the blue channel, [0,255]
        [FieldOffset(11)] public byte gain_b;
        // Offset (brightness) for the red channel, [0,63]
        [FieldOffset(12)] public byte offset_r;
        // Offset (brightness) for the green channel, [0,63]
        [FieldOffset(13)] public byte offset_g;
        // Offset (brightness) for the blue channel, [0,63]
        [FieldOffset(14)] public byte offset_b;
        // added for alignment, don't use
        [FieldOffset(15)] public byte reserved;
        // Shifts image up or down. Valid range depends on
        // the current video mode and the hardware capabilities.
        // Invalid values are rounded to the nearest valid value.
        [FieldOffset(16)] public int vshift;
        // PLL adjustment
        [FieldOffset(20)] public int pllshift;
        // Grab flags (V2U_GRAB_*)
        [FieldOffset(24)] public uint grab_flags;
        // Which bits from grab_flags are used
        [FieldOffset(28)] public uint grab_flags_mask;
    }

    /// <summary>
    /// Property keys
    /// </summary>
    public enum V2UPropertyKey : int
    {
        V2UKey_UsbProductID,            // int16     R
        V2UKey_ProductType,             // int32     R
        V2UKey_DirectShowFixRes,        // size      RW
        V2UKey_DirectShowFlags,         // int32     RW
        V2UKey_DirectShowDefaultBmp,    // wstr      RW
        V2UKey_ModeMeasurmentsDump,     // blob      R
        V2UKey_ResetADC,                // int32     W
        V2UKey_DirectShowScaleMode,     // scale     RW
        V2UKey_HardwareCompression,     // boolean   R
        V2UKey_AdjustmentsRange,        // adj_range R
        V2UKey_Version,                 // version   R
        V2UKey_EDID,                    // edid      RW
        V2UKey_DirectShowMaxFps,        // int32     RW
        V2UKey_KVMCapable,              // boolean   R
        V2UKey_VGAMode,                 // vgamode   RW
        V2UKey_CurrentVGAMode,          // vesa_mode R
        V2UKey_ModeMeasureInterval,     // int32     RW
        V2UKey_EDIDSupport,             // boolean   R
        V2UKey_ProductName,             // str       R
        V2UKey_TuneInterval,            // uint32    RW
        V2UKey_UserData,                // userdata  RW
        V2UKey_SerialNumber,            // str       R
        V2UKey_InputSignalType,         // uint32    R
        V2UKey_DigitalModeDetect,       // int32     RW
        V2UKey_NoiseFilter,             // int32     RW
        V2UKey_HSyncThreshold,          // uint8     RW
        V2UKey_VSyncThreshold,          // uin8      RW
        V2UKey_DeviceCaps               // uint32    R
    }

    // V2UKey_Version
    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct V2UVersion
    {
        [FieldOffset(0)]  public int major;
        [FieldOffset(4)]  public int minor;
        [FieldOffset(8)]  public int micro;
        [FieldOffset(12)] public int nano;
    }

    // V2UKey_ProductType
    public enum V2UProductType : int
    {
        V2UProductOther,
        V2UProductVGA2USB,
        V2UProductKVM2USB,
        V2UProductDVI2USB,
        V2UProductVGA2USBPro,
        V2UProductVGA2USBLR,
        V2UProductDVI2USBSolo,
        V2UProductDVI2USBDuo,
        V2UProductKVM2USBPro,
        V2UProductKVM2USBLR,
        V2UProductDVI2USBRespin,
        V2UProductVGA2USBHR,
        V2UProductVGA2USBLRRespin,
        V2UProductVGA2USBHRRespin,
        V2UProductVGA2USBProRespin,
        V2UProductVGA2FIFO,
        V2UProductKVM2FIFO,
        V2UProductDVI2FIFO,
        V2UProductDVI2Davinci1,
        V2UProductVGA2PCI,
        V2UProductKVM2USBLRRespin
    }

    // V2UKey_DigitalModeDetect
    public enum V2UDigitalModeDetect : int
    {
        V2UDigitalMode_AutoDetect,      // Automatic detection (default)
        V2UDigitalMode_SingleLink,      // Force single link
        V2UDigitalMode_DualLink,        // Force dual link
        V2UDigitalMode_Count            // Not a valid value
    }

    // V2UKey_NoiseFilter
    public enum V2UNoiseFilter : int
    {
        V2UNoiseFilter_Auto,            // Automatic configuration (default)
        V2UNoiseFilter_None,            // Disable noise filter
        V2UNoiseFilter_Low,
        V2UNoiseFilter_Moderate,
        V2UNoiseFilter_High,
        V2UNoiseFilter_Extreme 
    }

    // V2UKey_DirectShowScaleMode
    public enum V2UScaleMode : int
    {
        V2UScaleNone,
        V2UScaleModeNearestNeighbor,
        V2UScaleModeWeightedAverage,
        V2UScaleModeFastBilinear,
        V2UScaleModeBilinear,
        V2UScaleModeBicubic,
        V2UScaleModeExperimental,
        V2UScaleModePoint,
        V2UScaleModeArea,
        V2UScaleModeBicubLin,
        V2UScaleModeSinc,
        V2UScaleModeLanczos,
        V2UScaleModeSpline,
    }

    // V2UKey_CurrentVGAMode
    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct V2UVideoModeDescr
    {
        [FieldOffset(0)]  public uint   VerFrequency;
        [FieldOffset(4)]  public ushort HorAddrTime;
        [FieldOffset(6)]  public ushort HorFrontPorch;
        [FieldOffset(8)]  public ushort HorSyncTime;
        [FieldOffset(10)] public ushort HorBackPorch;
        [FieldOffset(12)] public ushort VerAddrTime;
        [FieldOffset(14)] public ushort VerFrontPorch;
        [FieldOffset(16)] public ushort VerSyncTime;
        [FieldOffset(18)] public ushort VerBackPorch;
        [FieldOffset(20)] public uint   Type;
    }

    // V2UKey_VGAMode
    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct V2UVGAMode
    {
        [FieldOffset(0)]  public int idx;
        [FieldOffset(4)]  public V2UVideoModeDescr vesa_mode;
    }

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct V2UPropertyValue
    {
        [FieldOffset(0)] public sbyte int8;
        [FieldOffset(0)] public byte uint8;
        [FieldOffset(0)] public short int16;
        [FieldOffset(0)] public ushort uint16;
        [FieldOffset(0)] public int int32;
        [FieldOffset(0)] public uint uint32;
        [FieldOffset(0)] public long int64;
        [FieldOffset(0)] public ulong uint64;
        [FieldOffset(0)] public int boolean;
        [FieldOffset(0)] public V2UProductType product_type;
        [FieldOffset(0)] public V2USize size;
        [FieldOffset(0)] public V2URect rect;
        [FieldOffset(0)] public V2UScaleMode scale;
        [FieldOffset(0)] public V2UVersion version;
        [FieldOffset(0)] public V2UVGAMode vgamode;
        [FieldOffset(0)] public fixed char str[256];
        [FieldOffset(0)] public fixed byte edid[128];
        [FieldOffset(0)] public fixed byte blob[256];
        [FieldOffset(0)] public fixed byte userdata[FrameGrabber.V2U_USERDATA_LEN];
    }

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct V2U_Property
    {
        [FieldOffset(0)] public V2UPropertyKey key;
        [FieldOffset(4)] public V2UPropertyValue value;
    }

    /// <summary>
    /// Represents a single frame grabber unit.
    /// </summary>
    public class FrameGrabber
    {
        public const int V2U_CUSTOM_VIDEOMODE_COUNT         = 8;
        public const int V2U_USERDATA_LEN                   = 8;

        // Product type bits
        public const int PRODUCT_ID_MASK = 0x0000ffff;
        public const int PRODUCT_TYPE_MASK                  = 0x00ff0000;
        public const int PRODUCT_TYPE_USB                   = 0x00010000;
        public const int PRODUCT_TYPE_NET                   = 0x00020000;
        public const int PRODUCT_TYPE_NETAPP                = 0x00030000;

        // V2UKey_DirectShowFlags
        public const int V2U_DSHOW_LIMIT_FPS                = 0x00000200;
        public const int V2U_DSHOW_FLIP_VERTICALLY          = 0x00000400;
        public const int V2U_DSHOW_FIX_FPS                  = 0x00000800;

        // Type of video input (V2UKey_InputSignalType)
        public const int V2U_INPUT_NONE                     = 0x00000000;
        public const int V2U_INPUT_ANALOG                   = 0x00000001;
        public const int V2U_INPUT_DIGITAL                  = 0x00000002;
        public const int V2U_INPUT_SOG                      = 0x00000004;
        public const int V2U_INPUT_COMPOSITE                = 0x00000008;

        // Device capabilities for V2UKey_DeviceCaps
        public const int V2U_CAPS_VGA_CAPTURE               = 0x00000001;
        public const int V2U_CAPS_DVI_CAPTURE               = 0x00000002;
        public const int V2U_CAPS_DVI_DUAL_LINK             = 0x00000004;
        public const int V2U_CAPS_KVM                       = 0x00000008;
        public const int V2U_CAPS_EDID                      = 0x00000010;
        public const int V2U_CAPS_HW_COMPRESSION            = 0x00000020;
        public const int V2U_CAPS_SYNC_THRESHOLD            = 0x00000040;

        // V2UVideoModeDescr flags
        public const int VIDEOMODE_TYPE_VALID               = 0x00000001;
        public const int VIDEOMODE_TYPE_ENABLED             = 0x00000002;
        public const int VIDEOMODE_TYPE_SUPPORTED           = 0x00000004;
        public const int VIDEOMODE_TYPE_DUALLINK            = 0x00000008;
        public const int VIDEOMODE_TYPE_DIGITAL             = 0x00000010;
        public const int VIDEOMODE_TYPE_INTERLACED          = 0x00000020;
        public const int VIDEOMODE_TYPE_HSYNCPOSITIVE       = 0x00000040;
        public const int VIDEOMODE_TYPE_VSYNCPOSITIVE       = 0x00000080;

        // Grab format options
        public const uint V2U_GRABFRAME_FLAGS_MASK          = 0xff000000;
        public const uint V2U_GRABFRAME_BOTTOM_UP_FLAG      = 0x80000000;
        public const uint V2U_GRABFRAME_KEYFRAME_FLAG       = 0x40000000;

        public const uint V2U_GRABFRAME_SCALE_MASK          = 0x000f0000;
        public const uint V2U_GRABFRAME_SCALE_NEAREST       = 0x00010000;
        public const uint V2U_GRABFRAME_SCALE_AVERAGE       = 0x00020000;
        public const uint V2U_GRABFRAME_SCALE_FAST_BILINEAR = 0x00030000;
        public const uint V2U_GRABFRAME_SCALE_BILINEAR      = 0x00040000;
        public const uint V2U_GRABFRAME_SCALE_BICUBIC       = 0x00050000;
        public const uint V2U_GRABFRAME_SCALE_EXPERIMENTAL  = 0x00060000;
        public const uint V2U_GRABFRAME_SCALE_POINT         = 0x00070000;
        public const uint V2U_GRABFRAME_SCALE_AREA          = 0x00080000;
        public const uint V2U_GRABFRAME_SCALE_BICUBLIN      = 0x00090000;
        public const uint V2U_GRABFRAME_SCALE_SINC          = 0x000A0000;
        public const uint V2U_GRABFRAME_SCALE_LANCZOS       = 0x000B0000;
        public const uint V2U_GRABFRAME_SCALE_SPLINE        = 0x000C0000;
        public const uint V2U_GRABFRAME_SCALE_MAX_MODE      = 0x000C0000;

        public const uint V2U_GRABFRAME_FORMAT_MASK         = 0x0000ffff;
        public const uint V2U_GRABFRAME_FORMAT_RGB_MASK     = 0x0000001f;
        public const uint V2U_GRABFRAME_FORMAT_RGB4         = 0x00000004;
        public const uint V2U_GRABFRAME_FORMAT_RGB8         = 0x00000008;
        public const uint V2U_GRABFRAME_FORMAT_RGB16        = 0x00000010;
        public const uint V2U_GRABFRAME_FORMAT_RGB24        = 0x00000018;
        public const uint V2U_GRABFRAME_FORMAT_YUY2         = 0x00000100;
        public const uint V2U_GRABFRAME_FORMAT_YV12         = 0x00000200;
        public const uint V2U_GRABFRAME_FORMAT_2VUY         = 0x00000300;
        public const uint V2U_GRABFRAME_FORMAT_BGR16        = 0x00000400;
        public const uint V2U_GRABFRAME_FORMAT_Y8           = 0x00000500;
        public const uint V2U_GRABFRAME_FORMAT_BGR24        = 0x00000800;

        public const uint V2U_FLAG_VALID_HSHIFT             = 0x00000001;
        public const uint V2U_FLAG_VALID_PHASE              = 0x00000002;
        public const uint V2U_FLAG_VALID_OFFSETGAIN         = 0x00000004;
        public const uint V2U_FLAG_VALID_VSHIFT             = 0x00000008;
        public const uint V2U_FLAG_VALID_PLLSHIFT           = 0x00000010;
        public const uint V2U_FLAG_VALID_GRABFLAGS          = 0x00000020;

        public const uint V2U_GRAB_BMP_BOTTOM_UP            = 0x00010000;
        public const uint V2U_GRAB_PREFER_WIDE_MODE         = 0x00020000;

        // Functions imported from frmgrab.dll
        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern int FrmGrabLocal_Count();

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern int FrmGrabLocal_OpenAll(IntPtr[] grabbers, int maxcount);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe IntPtr FrmGrabLocal_Open();

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Ansi)]
        static extern unsafe IntPtr FrmGrabLocal_OpenSN(string sn);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl,
            CharSet = CharSet.Ansi)]
        static extern unsafe IntPtr FrmGrabNet_OpenSN(string sn);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe IntPtr FrmGrabNet_OpenAddress(uint ipaddr, ushort port);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe void FrmGrab_Close(IntPtr fg);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe int FrmGrab_GetProductId(IntPtr fg);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe IntPtr FrmGrab_GetSN(IntPtr fg);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe IntPtr FrmGrab_GetProductName(IntPtr fg);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe IntPtr FrmGrab_GetLocation(IntPtr fg);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe uint FrmGrab_GetCaps(IntPtr fg);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe bool FrmGrab_DetectVideoMode(IntPtr fg, void* vm);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe bool FrmGrab_GetGrabParams(IntPtr fg, void* gp);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe bool FrmGrab_SetGrabParams(IntPtr fg, void* gp);
        
        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe bool FrmGrab_GetProperty(IntPtr fg, void* prop);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe bool FrmGrab_SetProperty(IntPtr fg, void* prop);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe bool FrmGrab_Start(IntPtr fg);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe void FrmGrab_Stop(IntPtr fg);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe bool FrmGrab_SetMaxFps(IntPtr fg, double maxFps);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe V2U_GrabFrame2* FrmGrab_Frame(IntPtr fg, uint format, V2URect* crop);

        [DllImport("frmgrab.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe void FrmGrab_Release(IntPtr fg, V2U_GrabFrame2* frame);

        private IntPtr iGrabber;

        /// <summary>
        /// Returns an integer that indicates the number of frame grabbers
        /// connected locally.
        /// </summary>
        public static int GetNumberOfLocalFrameGrabbers()
        {
            return FrmGrabLocal_Count();
        }

        /// <summary>
        /// Returns all frame grabbers that are attached locally.
        /// </summary>
        public static FrameGrabber[] OpenAllLocalFrameGrabbers()
        {
            int count = FrmGrabLocal_Count();
            IntPtr[] ptrs = new IntPtr[count];
            int numberOpened = FrmGrabLocal_OpenAll(ptrs, count);
            FrameGrabber[] frameGrabbers = new FrameGrabber[numberOpened];
            for (int i=0; i<numberOpened; i++) {
                frameGrabbers[i] = new FrameGrabber(ptrs[i]);
            }
            return frameGrabbers;
        }

        /// <summary>
        /// Creates a FrameGrabber instance.
        /// </summary>
        private FrameGrabber(IntPtr aGrabber)
        {
            if (aGrabber == IntPtr.Zero) {
                throw new Exception("Invalid argument");
            }
            iGrabber = aGrabber;
        }

        /// <summary>
        /// Creates a FrameGrabber instance that represents a local frame
        /// grabber. Throws Exception if no grabber was found
        /// </summary>
        public FrameGrabber()
        {
            iGrabber = FrmGrabLocal_Open();
            if (iGrabber == IntPtr.Zero) {
                throw new Exception("No local frame grabber found");
            }
        }

        /// <summary>
        /// Creates a FrameGrabber instance that represents a frame grabber
        /// with the specified serial number. It tries to find a local grabber
        /// first, and if the requested unit is not found locally, looks on
        /// the network. Throws Exception if the requested frame grabber is
        /// not currently available.
        /// </summary>
        public FrameGrabber(string aSerialNumber)
        {
            iGrabber = FrmGrabLocal_OpenSN(aSerialNumber);
            if (iGrabber == IntPtr.Zero) {
                iGrabber = FrmGrabNet_OpenSN(aSerialNumber);
                if (iGrabber == IntPtr.Zero) {
                    throw new Exception("Frame grabber #" + aSerialNumber +
                        " is not found");
                }
            }
        }

        /// <summary>
        /// Creates a FrameGrabber instance that represents either local or
        /// network frame grabber with the specified serial number. The type
        /// of the frame grabber to look for (local vs network) is determined
        /// by the second parameter. Throws Exception if the requested frame
        /// grabber is not currently available.
        /// </summary>
        public FrameGrabber(string aSerialNumber, bool aNetwork)
        {
            iGrabber = aNetwork ? FrmGrabNet_OpenSN(aSerialNumber) :
                FrmGrabLocal_OpenSN(aSerialNumber);
            if (iGrabber == IntPtr.Zero) {
                throw new Exception("Frame grabber #" + aSerialNumber +
                    " is not found");
            }
        }

        /// <summary>
        /// Creates a FrameGrabber instance that represents a network frame
        /// grabber with the specified IP address. Throws Exception if it
        /// can't connect to the grabber.
        /// </summary>
        public FrameGrabber(IPAddress aAddress) : this(aAddress, 0)
        {
        }

        /// <summary>
        /// Creates a FrameGrabber instance that represents a network frame
        /// grabber with the specified IP address. Throws Exception if it
        /// can't connect to the grabber.
        /// </summary>
        public FrameGrabber(IPAddress aAddress, ushort aPort)
        {
            byte[] bytes = aAddress.GetAddressBytes();
            if (bytes.Length.Equals(4)) {
                uint addr = BitConverter.ToUInt32(bytes, 0);
                if (BitConverter.IsLittleEndian) {
                    addr = (uint)IPAddress.NetworkToHostOrder((int)addr);
                }
                iGrabber = FrmGrabNet_OpenAddress(addr, aPort);
                if (iGrabber == IntPtr.Zero) {
                    throw new SocketException();
                }
            } else {
                throw new ArgumentException("aAddress");
            }
        }

        ~FrameGrabber()
        {
            FrmGrab_Close(iGrabber);
            iGrabber = IntPtr.Zero;
        }

        /// <summary>
        /// Returns the unique product id. The product id includes product
        /// type bits OR'ed with type-specific product id.
        /// </summary>
        public int GetProductId()
        {
            return FrmGrab_GetProductId(iGrabber);
        }

        /// <summary>
        /// Return true is this is a network grabber.
        /// </summary>
        public bool IsNetworkGrabber()
        {
            return ((GetProductId() & PRODUCT_TYPE_MASK) == PRODUCT_TYPE_NET);
        }

        /// <summary>
        /// Return true is this is a local USB grabber.
        /// </summary>
        public bool IsUSBGrabber()
        {
            return ((GetProductId() & PRODUCT_TYPE_MASK) == PRODUCT_TYPE_USB);
        }

        /// <summary>
        /// Return true is this is a local grabber.
        /// </summary>
        public bool IsLocalGrabber()
        {
            return IsUSBGrabber();
        }

        /// <summary>
        /// Returns frame grabber's serial number.
        /// </summary>
        public string GetSN()
        {
            return Marshal.PtrToStringAnsi(FrmGrab_GetSN(iGrabber));
        }

        /// <summary>
        /// Returns the product description ("VGA2USB", "VGA2Ethernet", etc)
        /// </summary>
        public string GetProductName()
        {
            return Marshal.PtrToStringAnsi(FrmGrab_GetProductName(iGrabber));
        }

        /// <summary>
        ///  Returns a string that describes the location of the grabber
        /// ("USB","192.168.0.122", etc)
        /// </summary>
        public string GetLocation()
        {
            return Marshal.PtrToStringAnsi(FrmGrab_GetLocation(iGrabber));
        }

        /// <summary>
        /// Returns the device capabilities
        /// </summary>
        public unsafe uint GetCaps()
        {
            return FrmGrab_GetCaps(iGrabber);
        }

        /// <summary>
        /// Detects current video mode. Return null if video mode detection
        /// fails. If no signal is detected, returns VideoMode object with
        /// width and height equal zero.
        /// </summary>
        public unsafe VideoMode DetectVideoMode()
        {
            V2U_VideoMode vm;
            if (FrmGrab_DetectVideoMode(iGrabber, &vm)) {
                return new VideoMode(vm);
            } else {
                return null;
            }
        }

        /// <summary>
        /// Queries current VGA capture parameters.
        /// </summary>
        public unsafe GrabParams GetGrabParams()
        {
            GrabParams gp = new GrabParams();
            fixed (V2U_GrabParameters* ptr = &gp.iGp) {
                if (FrmGrab_GetGrabParams(iGrabber, ptr)) {
                    return gp;
                } else {
                    return null;
                }
            }
        }

        /// <summary>
        /// Sets VGA capture parameters.
        /// </summary>
        public unsafe bool SetGrabParams(V2U_GrabParameters* aGp)
        {
            return FrmGrab_SetGrabParams(iGrabber, aGp);
        }

        /// <summary>
        /// Sets VGA capture parameters.
        /// </summary>
        public unsafe bool SetGrabParams(GrabParams aGp)
        {
            fixed (V2U_GrabParameters* gp = &aGp.iGp) {
                return SetGrabParams(gp);
            }
        }

        /// <summary>
        /// Queries the device property.
        /// </summary>
        public unsafe bool GetProperty(V2U_Property* aProperty)
        {
            return FrmGrab_GetProperty(iGrabber, aProperty);
        }

        /// <summary>
        /// Sets the device property.
        /// </summary>
        public unsafe bool SetProperty(V2U_Property* aProperty)
        {
            return FrmGrab_SetProperty(iGrabber, aProperty);
        }

        /// <summary>
        /// Sets the intended frame rate limit (average number of GrabFrame
        /// calls per second). The frame grabber may use this hint to reduce
        /// resource usage, especially in a low fps case.
        /// </summary>
        public bool SetMaxFps(double aMaxFps)
        {
            return FrmGrab_SetMaxFps(iGrabber, aMaxFps);
        }

        /// <summary>
        /// Signals the grabber to prepare for capturing frames with maximum
        /// frame rate. While it currently doesn't matter for local grabbers,
        /// it's really important for network grabbers (it turns streaming on,
        /// otherwise GrabFrame will have to work on request/response basis,
        /// which is much slower).
        /// </summary>
        public bool Start()
        {
            return FrmGrab_Start(iGrabber);
        }

        /// <summary>
        /// Signals the grabber to stop capturing frames with maximum rate.
        /// </summary>
        public void Stop()
        {
            FrmGrab_Stop(iGrabber);
        }

        /// <summary>
        /// Grabs one frame. The caller doesn't have to call Start first,
        /// but it's recommended in order to get maximum frame rate.
        /// </summary>
        public unsafe Frame GrabFrame()
        {
            return GrabFrame(V2U_GRABFRAME_FORMAT_BGR24, null);
        }

        /// <summary>
        /// Grabs one frame. The caller doesn't have to call Start first,
        /// but it's recommended in order to get maximum frame rate. 
        ///
        /// The second parameter is the capture format, i.e. one of
        /// V2U_GRABFRAME_FORMAT_* optionally combined with flags
        /// </summary>
        public unsafe Frame GrabFrame(uint format)
        {
            return GrabFrame(format, null);
        }

        /// <summary>
        /// Grabs one frame. The caller doesn't have to call Start first,
        /// but it's recommended in order to get maximum frame rate. 
        ///
        /// The second parameter is the capture format, i.e. one of
        /// V2U_GRABFRAME_FORMAT_* optionally combined with flags
        ///
        /// The last parameter is a pointer to the requested crop rectangle.
        /// Pass null if you need the whole frame.
        /// </summary>
        public unsafe Frame GrabFrame(uint format, V2URect* crop)
        {
            V2U_GrabFrame2* frame = FrmGrab_Frame(iGrabber, format, crop);
            if (frame != null) {
                try {
                    return new Frame(this, frame);
                } catch {
                    FrmGrab_Release(iGrabber, frame);
                }
            }
            return null;
        }

        /// <summary>
        /// Releases the frame previously returned by FrmGrab_Frame
        /// </summary>
        internal unsafe void ReleaseFrame(V2U_GrabFrame2* aFrame)
        {
            FrmGrab_Release(iGrabber, aFrame);
        }

        /// <summary>
        /// Returns a string describing this frame grabber.
        /// </summary>
        public override string ToString()
        {
            return String.Format("{0} [{1}]", GetProductName(), GetSN());
        }
    }

    /// <summary>
    /// A single frame captured by the frame grabber
    /// </summary>
    public unsafe class Frame
    {
        private FrameGrabber iGrabber;
        private V2U_GrabFrame2* iFrame;
        private Bitmap iBitmap;

        [DllImport("msvcrt.dll",
            CallingConvention = CallingConvention.Cdecl)]
        static extern unsafe void memcpy(IntPtr dest, IntPtr src, int length);

        internal Frame(FrameGrabber aGrabber, V2U_GrabFrame2* aFrame)
        {
            iGrabber = aGrabber;
            iFrame = aFrame;
            iBitmap = null;
        }

        void Release()
        {
            if (iGrabber != null) {
                iGrabber.ReleaseFrame(iFrame);
                iGrabber = null;
                iFrame = null;
                iBitmap = null;
            }
        }

        /// <summary>
        /// Returns Bitmap representation of this frame.
        /// </summary>
        public Bitmap GetBitmap()
        {
            if (iBitmap == null && iFrame != null) {
                int Bpp = 0;
                PixelFormat pixelFormat = PixelFormat.Undefined;
                uint frameFormat = (iFrame->palette &
                    FrameGrabber.V2U_GRABFRAME_FORMAT_MASK);

                switch (frameFormat) {
                case FrameGrabber.V2U_GRABFRAME_FORMAT_BGR24:
                    pixelFormat = PixelFormat.Format24bppRgb;
                    Bpp = 3;
                    break;
                case FrameGrabber.V2U_GRABFRAME_FORMAT_RGB16:
                    pixelFormat = PixelFormat.Format16bppRgb565;
                    Bpp = 2;
                    break;
                default:
                    Debug.Print("frmgrab: unsupported pixel format {0,8:x}",
                        frameFormat);
                    break;
                }
                if (pixelFormat != PixelFormat.Undefined) {
                    int stride = iFrame->crop.width * Bpp;
                    int len = iFrame->crop.height * stride;

                    //
                    // We could have done this:
                    //
                    // iBitmap = new Bitmap(iFrame->crop.width, len,
                    //    pixelFormat, new IntPtr(iFrame->pixbuf));
                    //
                    // and it would be more efficient because it wouldn't
                    // require copying the entire frame. However, in that
                    // case we would have to make sure that the Bitmap
                    // doesn't get garbage-collected before the frame gets
                    // released. There doesn't seem to be a clean way to do
                    // that. So let's play it safe and just copy the data.
                    //

                    iBitmap = new Bitmap(iFrame->crop.width,
                        iFrame->crop.height, pixelFormat);
                    BitmapData bmpData = iBitmap.LockBits(new Rectangle
                        (0, 0, iBitmap.Width, iBitmap.Height),
                        ImageLockMode.ReadWrite, iBitmap.PixelFormat);
                    if (bmpData.Stride == stride) {
                        memcpy(bmpData.Scan0, new IntPtr(iFrame->pixbuf), len);
                        iBitmap.UnlockBits(bmpData);
                    } else {
                        // Well, in this case we would need to copy the data
                        // line by line... But hey, this is just a sample
                        iBitmap.UnlockBits(bmpData);
                        iBitmap = null;
                    }
                }
            }
            return iBitmap;
        }

        ~Frame()
        {
            Release();
        }
    }

    /// <summary>
    /// Video mode information
    /// </summary>
    public class VideoMode
    {
        private V2U_VideoMode iVideoMode;
        public VideoMode(V2U_VideoMode aVideoMode)
        {
            iVideoMode = aVideoMode;
        }

        public int GetWidth()
        {
            return iVideoMode.width;
        }

        public int GetHeight()
        {
            return iVideoMode.height;
        }
        
        public float GetFrequency()
        {
            return iVideoMode.vfreq / 1000.0f;
        }

        public bool IsValid()
        {
            return iVideoMode.width > 0 && iVideoMode.height > 0;
        }

        public override string ToString()
        {
            return IsValid() ? 
                String.Format("{0:d}x{1:d} {2:d}.{3:d} Hz",
                iVideoMode.width, iVideoMode.height,
                (iVideoMode.vfreq+50)/1000,((iVideoMode.vfreq+50)%1000)/100) :
                "No signal";
        }
    }

    /// <summary>
    /// Wrapper for VGA capture parameters
    /// </summary>
    public class GrabParams
    {
        public V2U_GrabParameters iGp;
        public bool ManualHShift()
        {
            return (iGp.flags & FrameGrabber.V2U_FLAG_VALID_HSHIFT) != 0;
        }
        public bool ManualPhase()
        {
            return (iGp.flags & FrameGrabber.V2U_FLAG_VALID_PHASE) != 0;
        }
        public bool ManualOffsetGain()
        {
            return (iGp.flags & FrameGrabber.V2U_FLAG_VALID_OFFSETGAIN) != 0;
        }
        public bool ManualVShift()
        {
            return (iGp.flags & FrameGrabber.V2U_FLAG_VALID_VSHIFT) != 0;
        }
        public bool ManualPLL()
        {
            return (iGp.flags & FrameGrabber.V2U_FLAG_VALID_PLLSHIFT) != 0;
        }
        public void SetManualHShift(bool aManual)
        {
            if (aManual) {
                iGp.flags |= FrameGrabber.V2U_FLAG_VALID_HSHIFT;
            } else {
                iGp.flags &= ~FrameGrabber.V2U_FLAG_VALID_HSHIFT;
            }
        }
        public void SetManualPhase(bool aManual)
        {
            if (aManual) {
                iGp.flags |= FrameGrabber.V2U_FLAG_VALID_PHASE;
            } else {
                iGp.flags &= ~FrameGrabber.V2U_FLAG_VALID_PHASE;
            }
        }
        public void SetManualOffsetGain(bool aManual)
        {
            if (aManual) {
                iGp.flags |= FrameGrabber.V2U_FLAG_VALID_OFFSETGAIN;
            } else {
                iGp.flags &= ~FrameGrabber.V2U_FLAG_VALID_OFFSETGAIN;
            }
        }
        public void SetManualVShift(bool aManual)
        {
            if (aManual) {
                iGp.flags |= FrameGrabber.V2U_FLAG_VALID_VSHIFT;
            } else {
                iGp.flags &= ~FrameGrabber.V2U_FLAG_VALID_VSHIFT;
            }
        }
        public void SetManualPLL(bool aManual)
        {
            if (aManual) {
                iGp.flags |= FrameGrabber.V2U_FLAG_VALID_PLLSHIFT;
            } else {
                iGp.flags &= ~FrameGrabber.V2U_FLAG_VALID_PLLSHIFT;
            }
        }
        public byte Offset()
        {
            return (byte)(((int)iGp.offset_r + iGp.offset_g + iGp.offset_b) / 3);
        }
        public byte Gain()
        {
            return (byte)(((int)iGp.gain_r + iGp.gain_g + iGp.gain_b) / 3);
        }
        public void SetOffset(byte aOffset)
        {
            iGp.offset_r = iGp.offset_g = iGp.offset_b = aOffset;
        }
        public void SetGain(byte aGain)
        {
            iGp.gain_r = iGp.gain_g = iGp.gain_b = aGain;
        }
    }
}
