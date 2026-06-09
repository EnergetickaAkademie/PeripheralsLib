#ifndef BARGRAPH_H
#define BARGRAPH_H

#include "shift_register_device.h"

enum class BargraphMode {
	NORMAL,
	CENTER
};

/**
 * @brief A flexible bargraph device controlled by one or more shift registers.
 */
class Bargraph : public ShiftRegisterDevice {
public:
	/**
	 * @brief Construct a new Bargraph object.
	 * @param numLeds The total number of LEDs in the bargraph.
	 */
	Bargraph(uint8_t numLeds = 16);

	/**
	 * @brief Destructor to free the dynamically allocated memory.
	 */
	~Bargraph();

	/**
	 * @brief Sets the operating mode of the bargraph.
	 * @param mode The display mode (NORMAL or CENTER).
	 */
	void setMode(BargraphMode mode);

	/**
	 * @brief Sets the value range for the bargraph.
	 * Useful for mapping values in NORMAL or CENTER modes.
	 * @param minVal The minimum expected value.
	 * @param maxVal The maximum expected value.
	 */
	void setRange(int minVal, int maxVal);

	/**
	 * @brief Sets the value to display on the bargraph.
	 * @param value The value to display.
	 */
	void setValue(int value);

	/**
	 * @brief Sets the display direction of the bargraph.
	 * @param reversed If true, the bargraph will fill from top to bottom.
	 */
	void setReversed(bool reversed);

	/**
	 * @brief Sets the raw byte data for the bargraph LEDs.
	 * @param data A pointer to an array of bytes representing the LED states.
	 * @param count The number of bytes in the data array.
	 */
	void setRawData(const byte* data, uint8_t count);

	/**
	 * @brief Turns the bargraph on or off completely.
	 * @param enabled If true, the bargraph shows LEDs. If false, all LEDs are off.
	 */
	void setEnabled(bool enabled);

	/**
	 * @brief Gets the current enabled state of the bargraph.
	 * @return true if the bargraph is enabled, false if turned off.
	 */
	bool isEnabled() const;

	// --- Implementations for the ShiftRegisterDevice interface ---
	const byte* getShiftData() const override;
	uint8_t getRegisterCount() const override;

private:
	uint8_t _numLeds;
	uint8_t _registerCount;
	byte* _shiftData;
	bool _reversed;
	bool _enabled;

	BargraphMode _mode;
	int _minVal;
	int _maxVal;
};

#endif // BARGRAPH_H
