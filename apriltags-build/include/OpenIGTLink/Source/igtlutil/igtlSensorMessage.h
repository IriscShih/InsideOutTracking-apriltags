/*=========================================================================

  Program:   The OpenIGTLink Library
  Language:  C++
  Web page:  http://openigtlink.org/

  Copyright (c) Insight Software Consortium. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __igtlSensorMessage_h
#define __igtlSensorMessage_h

#include <string>

#include "igtlObject.h"
#include "igtlMath.h"
#include "igtlMessageBase.h"
#include "igtlTypes.h"

#include "igtlUnit.h"

namespace igtl
{

/// SENSOR is a message type, which is used to transfer sensor reading,
/// 3-axis position, velocity, acceleration, angle, angle velocity and angle acceleration.
/// The message format is intended for manipulator control and various types of sensors.
class IGTLCommon_EXPORT SensorMessage: public MessageBase
{
public:
  typedef SensorMessage                  Self;
  typedef MessageBase                    Superclass;
  typedef SmartPointer<Self>             Pointer;
  typedef SmartPointer<const Self>       ConstPointer;

  igtlTypeMacro(igtl::SensorMessage, igtl::MessageBase);
  igtlNewMacro(igtl::SensorMessage);

public:

  /// Sets the length of the array. Returns the resulted length.
  int          SetLength(unsigned int n);

  /// Gets the length of the array.
  unsigned int GetLength();

  /// Sets the unit for the sensor values. Returns 1 if success.
  int          SetUnit(igtlUnit unit);

  /// Sets the unit for the sensor values Returns 1 if success.
  int          SetUnit(igtl::Unit * unit);

  /// Gets the unit as 64-bit unit field defined in igtlUnit.h.
  igtlUnit     GetUnit();

  /// Gets the unit as igtl::Unit class.
  int          GetUnit(igtl::Unit * unit);

  /// Sets sensor values from an array of 64-bit floating data. Returns 1 if success.
  int          SetValue(igtlFloat64 * data);

  /// Sets the value for the i-th sensor. Returns 1 if success.
  int          SetValue(unsigned int i, igtlFloat64 value);

  /// Gets the value of the i-th sensor.
  igtlFloat64  GetValue(unsigned int i);

protected:
  SensorMessage();
  ~SensorMessage();
  
protected:

  virtual int  GetBodyPackSize();
  virtual int  PackBody();
  virtual int  UnpackBody();
  
  /// Length of sensor array.
  igtlUint8    m_Length;

  /// Sensor status (Reserved).
  igtlUint8    m_Status;

  /// Unit (generated by igtl::Unit::Pack()).
  igtlUnit     m_Unit;
  
  /// The array of sensor values.
  std::vector<igtlFloat64> m_Array;
};


} // namespace igtl

#endif // _igtlSensorMessage_h


