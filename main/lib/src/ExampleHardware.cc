#include "eudaq/ExampleHardware.hh"
#include "eudaq/Utils.hh"

namespace eudaq {

  namespace {

    std::vector<unsigned char> MakeRawEvent(unsigned width, unsigned height) {
      std::vector<unsigned char> result(width * height * 2);
      size_t offset = 0;

      unsigned hotX=2, hotY=42;
      for (unsigned x = 0; x < width; ++x) {
	for (unsigned y = 0; y < height; ++y) {
          unsigned short charge = std::rand()%300;
	  // Make a hot spot:
	  if ( (abs(x-hotX) + abs(y-hotY)) < 2)
	    charge+=22;
	  if ((abs(x-hotX) + abs(y-hotY)) > 3)
	    charge = charge/10.0;
	  if ((abs(x-hotX) + abs(y-hotY)) > 5)
	    charge=1;

          setlittleendian(&result[offset], charge);
          offset += 2;
        }
      }
      return result;
    }

    std::vector<unsigned char>
    ZeroSuppressEvent(const std::vector<unsigned char> &data, unsigned width,
                      int threshold = 30) {
      unsigned height = data.size() / width / 2;
      std::vector<unsigned char> result;
      size_t inoffset = 0, outoffset = 0;
      for (unsigned x = 0; x < width; ++x) {
	for (unsigned y = 0; y < height; ++y) {
	  unsigned short charge = getlittleendian<unsigned short>(&data[inoffset]);
          if (charge > threshold) {
            result.resize(outoffset + 6);
            setlittleendian<unsigned short>(&result[outoffset + 0], x);
            setlittleendian<unsigned short>(&result[outoffset + 2], y);
            setlittleendian<unsigned short>(&result[outoffset + 4], charge);
            outoffset += 6;
          }
          inoffset += 2;
        }
      }
      return result;
    }
  }

  ExampleHardware::ExampleHardware()
      : m_numsensors(8), m_width(4), m_height(64), m_triggerid(0) {}

  void ExampleHardware::Setup(int) {}

  void ExampleHardware::PrepareForRun() {
    m_timer.Restart();
    m_nextevent = 10.0;
    m_triggerid = 0;
  }

  bool ExampleHardware::EventsPending() const {
    return m_timer.Seconds() > m_nextevent;
  }

  unsigned ExampleHardware::NumSensors() const { return m_numsensors; }

  std::vector<unsigned char> ExampleHardware::ReadSensor(int sensorid) {
    std::vector<unsigned char> result(8);
    setlittleendian<unsigned short>(&result[0], m_width);
    setlittleendian<unsigned short>(&result[2], m_height);
    setlittleendian<unsigned short>(&result[4], m_triggerid);
    if (sensorid % 2 == 0) {
      // Raw Data
      setlittleendian(&result[6], static_cast<unsigned short>(0));
      std::vector<unsigned char> data = MakeRawEvent(m_width, m_height);
      result.insert(result.end(), data.begin(), data.end());
    } else {
      // Zero suppressed data
      std::vector<unsigned char> data =
          ZeroSuppressEvent(MakeRawEvent(m_width, m_height), m_width);
      result.insert(result.end(), data.begin(), data.end());
      unsigned short numhits = (result.size() - 8) / 6;

      // APZ: I don't understand this line from the original code:
      //setlittleendian<unsigned short>(&result[6], 0x8000 | numhits);
      // This one seems to work correctly:
      setlittleendian<unsigned short>(&result[6], numhits);
    }
    return result;
  }

  void ExampleHardware::CompletedEvent() {
    m_triggerid++;
    m_nextevent += 1.0;
  }

} // namespace eudaq
