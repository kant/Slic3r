#ifndef slic3r_GUI_Utils_Serial_hpp_
#define slic3r_GUI_Utils_Serial_hpp_

#include <vector>
#include <string>
#include <boost/optional.hpp>
#include <boost/asio.hpp>


namespace Slic3r {
namespace Utils {

struct SerialPortInfo {
	std::string port;
	std::string hardware_id;
	std::string friendly_name;
	bool 		is_printer = false;
};

inline bool operator==(const SerialPortInfo &sp1, const SerialPortInfo &sp2)
{
	return sp1.port 	   == sp2.port 	      &&
		   sp1.hardware_id == sp2.hardware_id &&
		   sp1.is_printer  == sp2.is_printer;
}

extern std::vector<std::string> 	scan_serial_ports();
extern std::vector<SerialPortInfo> 	scan_serial_ports_extended();


class Serial : public boost::asio::serial_port
{
public:
	Serial(boost::asio::io_service &io_service);
	// This c-tor opens the port and sets a baud rate
	Serial(boost::asio::io_service &io_service, const std::string &name, unsigned baud_rate);
	Serial(Serial &&) = default;
	Serial(const Serial &) = delete;
	Serial &operator=(Serial &&) = default;
	Serial &operator=(const Serial &) = delete;
	~Serial();

	void set_baud_rate(unsigned baud_rate);
	void set_DTR(bool on);

	// Resets the line number both internally as well as with the firmware using M110
	void reset_line_num();

	// Reads a line or times out, the timeout is in milliseconds
	boost::optional<std::string> read_line(unsigned timeout);

	// Write Marlin-formatted line, with a line number and a checksum
	size_t printer_write_line(const std::string &line, unsigned line_num);

	// Same as above, but with internally-managed line number
	size_t printer_write_line(const std::string &line);

	// Formats a line Marlin-style, ie. with a sequential number and a checksum
	static std::string printer_format_line(const std::string &line, unsigned line_num);
private:
	enum { BUFFER_SIZE = 4096 };
	unsigned m_line_num = 0;
	boost::asio::streambuf m_buffer;
	boost::asio::deadline_timer m_timer;
};


} // Utils
} // Slic3r

#endif /* slic3r_GUI_Utils_Serial_hpp_ */
