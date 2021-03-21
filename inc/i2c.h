#ifndef I2C_H_
#define I2C_H_

/* Structure that contains identifier details used in example */
struct identifier
{
    uint8_t dev_addr; // Variable to hold device address
    int8_t fd;        //Variable that contains file descriptor
};

/****************************************************************************/
/*!                         Functions                                       */

/*!
 *  @brief Function that creates a mandatory delay required in some of the APIs.
 *
 * @param[in] period              : Delay in microseconds.
 * @param[in, out] intf_ptr       : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *  @return void.
 *
 */
void user_delay_us(uint32_t period, void *intf_ptr);

/*!
 * @brief Function for print the temperature, humidity and pressure data.
 *
 * @param[out] comp_data    :   Structure instance of bme280_data
 *
 * @note Sensor data whose can be read
 *
 * sens_list
 * --------------
 * Pressure
 * Temperature
 * Humidity
 *
 */
void print_sensor_data(struct bme280_data *comp_data);

/*!
 *  @brief Function for reading the sensor's registers through I2C bus.
 *
 *  @param[in] reg_addr       : Register address.
 *  @param[out] data          : Pointer to the data buffer to store the read data.
 *  @param[in] len            : No of bytes to read.
 *  @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs.
 *
 *  @return Status of execution
 *
 *  @retval 0 -> Success
 *  @retval > 0 -> Failure Info
 *
 */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);

/*!
 *  @brief Function for writing the sensor's registers through I2C bus.
 *
 *  @param[in] reg_addr       : Register address.
 *  @param[in] data           : Pointer to the data buffer whose value is to be written.
 *  @param[in] len            : No of bytes to write.
 *  @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *
 *  @return Status of execution
 *
 *  @retval BME280_OK -> Success
 *  @retval BME280_E_COMM_FAIL -> Communication failure.
 *
 */
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

/*!
 * @brief Function reads temperature, humidity and pressure data in forced mode.
 *
 * @param[in] dev   :   Structure instance of bme280_dev.
 *
 * @return Result of API execution status
 *
 * @retval BME280_OK - Success.
 * @retval BME280_E_NULL_PTR - Error: Null pointer error
 * @retval BME280_E_COMM_FAIL - Error: Communication fail error
 * @retval BME280_E_NVM_COPY_FAILED - Error: NVM copy failed
 *
 */
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev, double *temp_amb);

/*!
 * @brief This function mounts i2c bus structure
 *
 * @param[in] dev   :   Pointer to structure instance of bme280_dev.
 * @param[in] in   :   Pointer to tructure instance of struct identifier.
 *
 */
void monta_i2c(struct bme280_dev *dev, struct identifier *id);

/*!
 * @brief This function mounts i2c bus structure
 *
 * @param[in] dev   :   Pointer to tructure instance of bme280_dev.
 * @param[in] in   :   Pointer to tructure instance of struct identifier.
 *
 */
void abre_i2c(struct bme280_dev *dev, struct identifier *id);


void inicializa_bme280_i2c(struct bme280_dev *dev);

void configura_bme280_i2c(struct bme280_dev *dev);


/*!
 * @brief This function mounts i2c bus structure
 *
 * @param[in] dev   :   Pointer to structure instance of bme280_dev.
 * @param[in] temp_amb   :   Pointer to structure that keeps environment temperature.
 *
 */
double le_temp_bme280_i2c(struct bme280_dev *dev);

#endif /* I2C_H_ */