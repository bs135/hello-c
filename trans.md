# CREATE

## trans_create(id, tank_offset, type, data)

Initialize a new transaction with provided params.
Saves the transaction to a new file with filename like `i_<id>.bin`

    @id         : transaction id
    @tank_offset: offset of the tank
    @type       : transaction type
    @data       : transaction data (optional)

    @return     : OK=0 or error_code

| name                     | value | description |
| ------------------------ | ----- | ----------- |
| TRANS_TYPE_PUMP          | 1     |             |
| TRANS_TYPE_BYPASS_ON     | 2     |             |
| TRANS_TYPE_BYPASS_OFF    | 3     |             |
| TRANS_TYPE_BYPASS_NORMAL | 4     |             |

# READ

## trans_read(id)

get transaction by id

    @id         : transaction id

    @return     : transaction data or NULL

## trans_read_all()

get all transactions

    @return     : a list data of all transactions

## trans_count(type)

return the number of `type` transactions

    @return     : number

## trans_count_all()

return the number of all transactions

    @return     : number

## trans_has_in_progress()

Check if any transactions are in progress

# UPDATE

## trans_update(id, trans_data)

## trans_close(id, reason)

## trans_close_all_active(reason)

## trans_close_tank(tank_offset, reason)

Close any open transaction assigned to a specific tank.

| name                          | value | description                                                                                            |
| ----------------------------- | ----- | ------------------------------------------------------------------------------------------------------ |
| T_CLOSE_SUCCESS               | 0x00  | Normal transaction close due to pump_end command                                                       |
| T_CLOSE_OLD_TRANSACTION       | 0x01  | Unused, transactions with this value were created before the close reason implementation was added     |
| T_CLOSE_NOFLOW                | 0x02  | Transaction closed due to no flow timeout                                                              |
| T_CLOSE_NEW_TRANSACTION       | 0x03  | Transaction closed due to a new transaction (interrupted by another transaction on tank)               |
| T_CLOSE_SW_RESET              | 0x04  | Transaction closed due to software reset command                                                       |
| T_CLOSE_BUTTON_RESET          | 0x05  | Transaction closed due to button press/hold                                                            |
| T_CLOSE_TCP_DISCONNECT        | 0x06  | Transaction closed because TCP disconnect                                                              |
| T_CLOSE_POWER_CYCLE           | 0x07  | Transaction closed due to power cycle                                                                  |
| T_CLOSE_WDT_RESET             | 0x08  | Transaction closed due to WDT reset                                                                    |
| T_CLOSE_DEACTIVATED           | 0x09  | Transaction closed due to system deactivation (activate = false), only applies to tank 0               |
| T_CLOSE_HANDLE_DETECT         | 0x0A  | Transaction closed due to handle detect/reinsertion (V1.4 hardware only)                               |
| T_CLOSE_WIFI_DISCONNECT       | 0x0B  | Transaction closed due to WiFi disconnect (device disconnected from WiFi without closing socket first) |
| T_CLOSE_NOFLOW_CURRENT_DETECT | 0x0C  | HW V1.4, no flow timeout and pump current sensor > threshold                                           |
| T_CLOSE_LOW_CURRENT_DETECT    | 0x0D  | Transaction closed due to the pump current lower than threshold                                        |
| T_CLOSE_POWER_LOST            | 0x0E  | Transaction closed due to power lost                                                                   |
| T_CLOSE_LINK_DISCONNECT       | 0x0F  | Transaction closed due to the connection to the CloudLink is lost                                      |
| T_CLOSE_NOFLOW_NOCURRENT      | 0x10  | HW V1.4, no flow timeout and pump current sensor < threshold                                           |
| T_CLOSE_BLUE_DISCONNECT       | 0x11  | Transaction closed due to the BLUE connection is lost                                                  |
| T_CLOSE_PRESET_VOLUME         | 0x12  | Transaction closed due to reaching the volume limit                                                    |
| T_CLOSE_NOFLOW_NOVOLUME       | 0x13  | no flow timeout and no volume pumped                                                                   |
| T_CLOSE_DC_VOLTAGE_DROP       | 0x14  | DC Supply Voltage is dropped too low. (HW-269)                                                         |
| T_CLOSE_SWITCH_OFF            | 0x15  | Transaction closed due to switch bypass to OFF position                                                |
| T_CLOSE_NOT_CLOSED            | 0xFF  | Transaction is unused or open (not closed yet)                                                         |

# DELETE

## trans_delete(id)

## trans_delete_type(type)

## trans_delete_all()
