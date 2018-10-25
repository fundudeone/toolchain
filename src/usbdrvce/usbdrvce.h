/**
 * @file
 * @author Jacob "jacobly" Young
 * @brief USB driver
 */

#ifndef H_USBDRVCE
#define H_USBDRVCE

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum usb_init_flags {
  USB_USE_C_HEAP         = 1 << 0, /**< Use part of the default C heap.
                                        @warning Do not use this unless you
                                        changed your program's bss/heap to end
                                        at 0xD10000! */
  USB_USE_PERIODIC_LIST  = 1 << 1, /**< Use space normally used for periodic.
                                        @warning This disables support for
                                        interrupt transfers, isochronous
                                        transfers, and hubs! */
  USB_USE_USB_AREA       = 1 << 2, /**< Use the memory that TIOS uses for usb. */
  USB_USE_OS_HEAP        = 1 << 3, /**< Use the application heap area. */
  USB_DEFAULT_INIT_FLAGS = USB_USE_USB_AREA | USB_USE_OS_HEAP,
} usb_init_flags_t;

typedef enum usb_device_event {
  USB_DEVICE_DISCONNECTED,
  USB_DEVICE_CONNECTED,
  USB_DEVICE_DISABLED,
  USB_DEVICE_ENABLED,
  USB_DEVICE_OVERCURRENT_DEACTIVATED,
  USB_DEVICE_OVERCURRENT_ACTIVATED,
  USB_DEVICE_DEFAULT_SETUP,
} usb_device_event_t;

typedef enum usb_error {
  USB_SUCCESS,
  USB_IGNORE,
  USB_ERROR_SYSTEM,
  USB_ERROR_INVALID_PARAM,
  USB_ERROR_SCHEDULE_FULL,
  USB_ERROR_NO_DEVICE,
  USB_ERROR_NO_MEMORY,
  USB_ERROR_NOT_SUPPORTED,
  USB_ERROR_TIMEOUT,
  USB_USER_ERROR = 100,
} usb_error_t;

typedef enum usb_transfer_status {
  USB_TRANSFER_COMPLETED,    /**< Transfered successfully. @note A receive    */
                             /**  transfer will complete when the end of a    */
                             /**  packet is detected, or the buffer is        */
                             /**  filled, whichever happens first.            */
  USB_TRANSFER_ERROR,        /**< Transfer failed due to timout and/or        */
                             /**  corruption.                                 */
  USB_TRANSFER_TIMED_OUT,    /**< Max retry attempts exceeded.                */
  USB_TRANSFER_CANCELLED,    /**< Transfer was cancelled by the user.         */
  USB_TRANSFER_STALL,        /**< Endpoint halt condition detected or control */
                             /**  request not supported.                      */
  USB_TRANSFER_NO_DEVICE,    /**< The device was disconnected.                */
  USB_TRANSFER_OVERFLOW,     /**< Device sent more bytes than can be stored   */
                             /**  in the transfer buffer, and were therefore  */
                             /**  lost. @note This can be avoided by ensuring */
                             /**  that receive buffer lengths are always a    */
                             /**  multiple of the endpoint's maximum packet   */
                             /**  length.                                     */
  USB_TRANSFER_MEMORY_ERROR, /**< Memory could not be accessed in a timely    */
                             /**  enough fashion to complete the transfer.    */
                             /**  @note This probably means that non-default  */
                             /**  cpu speed or lcd parameters are in use.     */
  USB_TRANSFER_HOST_ERROR,   /**< The results of the transaction were         */
                             /**  missed due to host hold-off. @note This     */
                             /**  probably indicates a bug in this library.   */
} usb_transfer_status_t;

typedef enum usb_find_flag {
  USB_FIND_DISABLED = 1 << 0, /**< Only return disabled devices.              */
  USB_FIND_ENABLED  = 1 << 1, /**< Only return enabled devices.               */
  USB_FIND_DEVICE   = 1 << 2, /**< Only return non-hubs.                      */
  USB_FIND_HUB      = 1 << 3, /**< Only return hubs.                          */
  USB_FIND_DIRECT   = 1 << 4, /**< Only return devices directly attached to   */
                              /**  the specified \p root hub.                 */
} usb_find_flag_t;

typedef enum usb_speed {
  USB_SPEED_UNKNOWN = -1,
  USB_SPEED_FULL,             /**<  12 Mb/s                                    */
  USB_SPEED_LOW,              /**< 1.5 Mb/s                                    */
  USB_SPEED_HIGH,             /**< 480 Mb/s                                    */
  USB_SPEED_SUPER,            /**<   5 Gb/s                                    */
} usb_speed_t;

typedef enum usb_transfer_direction {
  USB_HOST_TO_DEVICE = 0 << 7,
  USB_DEVICE_TO_HOST = 1 << 7,
} usb_transfer_direction_t;

typedef enum usb_request_type {
  USB_STANDARD_REQUEST = 0 << 5,
  USB_CLASS_REQUEST    = 1 << 5,
  USB_VENDOR_REQUEST   = 2 << 5,
} usb_request_type_t;

typedef enum usb_request {
  USB_GET_STATUS,
  USB_CLEAR_FEATURE,
  USB_SET_FEATURE = 3,
  USB_SET_ADDRESS = 5,
  USB_GET_DESCRIPTOR,
  USB_SET_DESCRIPTOR,
  USB_GET_CONFIGURATION,
  USB_SET_CONFIGURATION,
  USB_GET_INTERFACE,
  USB_SET_INTERFACE,
  USB_SYNC_FRAME,
} usb_request_t;

typedef enum usb_descriptor_type {
  USB_DEVICE_DESCRIPTOR = 1,
  USB_CONFIGURATION_DESCRIPTOR,
  USB_STRING_DESCRIPTOR,
  USB_INTERFACE_DESCRIPTOR,
  USB_ENDPOINT_DESCRIPTOR,
  USB_DEVICE_QUALIFIER_DESCRIPTOR,
  USB_OTHER_SPEED_CONFIGURATION_DESCRIPTOR,
  USB_INTERFACE_POWER_DESCRIPTOR,
} usb_descriptor_type_t;

typedef enum usb_class {
  USB_INTERFACE_SPECIFIC_CLASS,
  USB_AUDIO_CLASS,
  USB_COMM_CLASS,
  USB_HID_CLASS,
  USB_PHYSICAL_CLASS = 5,
  USB_IMAGE_CLASS,
  USB_PRINTER_CLASS,
  USB_STORAGE_CLASS,
  USB_HUB_CLASS,
  USB_CDC_DATA_CLASS,
  USB_SMART_CARD_CLASS,
  USB_CONTENT_SECURITY_CLASS,
  USB_VIDEO_CLASS,
  USB_PERSONAL_HEALTCARE_CLASS,
  USB_AUDIO_VIDEO_CLASS,
  USB_BILLBOARD_CLASS,
  USB_TYPE_C_BRIDGE_CLASS,
  USB_DIAGNOSTIC_DEVICE_CLASS = 0xDC,
  USB_WIRELESS_CONTROLLER_CLASS = 0xE0,
  USB_MISCELLANEOUS_CLASS = 0xEF,
  USB_APPLICATION_SPECIFIC_CLASS = 0xFE,
  USB_VENDOR_SPECIFIC_CLASS = 0xFF,
} usb_class_t;

typedef enum usb_configuration_attributes {
  USB_BUS_POWERED              = 0 << 6,
  USB_SELF_POWERED             = 1 << 6,
  USB_NO_REMOTE_WAKEUP         = 0 << 5,
  USB_REMOTE_WAKEUP            = 1 << 5,
  USB_CONFIGURATION_ATTRIBUTES = 1 << 7,
} usb_configuration_attributes_t;

typedef enum usb_usage_type {
  USB_DATA_ENDPOINT                   = 0 << 4,
  USB_FEEDBACK_ENDPOINT               = 1 << 4,
  USB_IMPLICIT_FEEDBACK_DATA_ENDPOINT = 2 << 4,
} usb_usage_type_t;

typedef enum usb_synchronization_type {
  USB_NO_SYNCHRONIZATION = 0 << 2,
  USB_ASYNCHRONOUS       = 1 << 2,
  USB_ADAPTIVE           = 2 << 2,
  USB_SYNCHRONOUS        = 3 << 2,
} usb_synchronization_type_t;

typedef enum usb_transfer_type {
  USB_CONTROL_TRANSFER,
  USB_ISOCHRONOUS_TRANSFER,
  USB_BULK_TRANSFER,
  USB_INTERRUPT_TRANSFER,
} usb_transfer_type_t;

typedef struct usb_control_setup {
  uint8_t  bmRequestType;       /**< direction, type, and recipient           */
  uint8_t  bRequest;            /**< usb_request_t                            */
  uint16_t wValue;              /**< request specific                         */
  uint16_t wIndex;              /**< request specific                         */
  uint16_t wLength;             /**< transfer length                          */
} usb_control_setup_t;

typedef struct usb_descriptor {
  uint8_t  bLength;             /**< The length of this descriptor.           */
  uint8_t  bDescriptorType;     /**< A usb_descriptor_type_t.                 */
  uint8_t  data[1];             /**< The rest of the descriptor               */
} usb_descriptor_t;

typedef struct usb_device_descriptor {
  uint8_t  bLength;             /**< 18                                       */
  uint8_t  bDescriptorType;     /**< USB_DEVICE_DESCRIPTOR                    */
  uint16_t bcdUSB;              /**< usb specification version                */
  uint8_t  bDeviceClass;        /**< usb_class_t                              */
  uint8_t  bDeviceSubClass;     /**< usb class specific                       */
  uint8_t  bDeviceProtocol;     /**< usb class specific                       */
  uint8_t  bMaxPacketSize0;     /**< 8, 16, 32, or 64                         */
  uint16_t idVendor;            /**< usb assigned vendor id                   */
  uint16_t idProduct;           /**< usb assigned product id                  */
  uint16_t bcdDevice;           /**< device version                           */
  uint8_t  iManufacturer;       /**< index of manufacturer string descriptor  */
  uint8_t  iProduct;            /**< index of product string descriptor       */
  uint8_t  iSerialNumber;       /**< index of serial number string descriptor */
  uint8_t  bNumConfigurations;  /**< how many valid configuration indices     */
} usb_device_descriptor_t;

typedef struct usb_device_qualifier_descriptor {
  uint8_t  bLength;             /**< 10                                       */
  uint8_t  bDescriptorType;     /**< USB_DEVICE_QUALIFIER_DESCRIPTOR          */
  uint16_t bcdUSB;              /**< usb specification version                */
  uint8_t  bDeviceClass;        /**< usb_class_t                              */
  uint8_t  bDeviceSubClass;     /**< usb class specific                       */
  uint8_t  bDeviceProtocol;     /**< usb class specific                       */
  uint8_t  bMaxPacketSize0;     /**< 8, 16, 32, or 64                         */
  uint8_t  bNumConfigurations;  /**< how many valid configuration indices     */
  uint8_t  bReserved;           /**< must be 0                                */
} usb_device_qualifier_descriptor_t;

typedef struct usb_configuration_descriptor {
  uint8_t  bLength;             /**< 9                                        */
  uint8_t  bDescriptorType;     /**< USB_CONFIGURATION_DESCRIPTOR             */
  uint16_t wTotalLength;        /**< total length of combined descriptors     */
  uint8_t  bNumInterfaces;      /**< how many interface descriptors follow    */
  uint8_t  bConfigurationValue; /**< value used to select this configuration  */
  uint8_t  iConfiguration;      /**< index of description string descriptor   */
  uint8_t  bmAttributes;        /**< usb_configuration_attributes_t           */
  uint8_t  bMaxPower;           /**< units of 2mA                             */
} usb_configuration_descriptor_t;
typedef struct usb_configuration_descriptor usb_other_speed_configuration_t;

typedef struct usb_interface_descriptor {
  uint8_t  bLength;             /**< 9                                        */
  uint8_t  bDescriptorType;     /**< USB_INTERFACE_DESCRIPTOR                 */
  uint8_t  bInterfaceNumber;    /**< zero-based interface index               */
  uint8_t  bAlternateSetting;   /**< value used to select this alt setting    */
  uint8_t  bNumEndpoints;       /**< how many endpoint descriptors follow     */
  uint8_t  bInterfaceClass;     /**< usb_class_t                              */
  uint8_t  bInterfaceSubClass;  /**< usb class specific                       */
  uint8_t  bInterfaceProtocol;  /**< usb class specific                       */
  uint8_t  iInterface;          /**< index of description string descriptor   */
} usb_interface_descriptor_t;

typedef struct usb_endpoint_descriptor {
  uint8_t  bLength;             /**< 7                                        */
  uint8_t  bDescriptorType;     /**< USB_ENDPOINT_DESCRIPTOR                  */
  uint8_t  bEndpointAddress;    /**< endpoint direction and number            */
  uint8_t  bmAttributes;        /**< usb_usage_type_t |                       */
                                /**  usb_synchronization_type_t |             */
                                /**  usb_transfer_type_t                      */
  uint16_t wMaxPacketSize;      /**  transfer type specific                   */
  uint8_t  bInterval;           /**  transfer type specific                   */
} usb_endpoint_descriptor_t;

typedef struct usb_string_descriptor {
  uint8_t  bLength;             /**< byte length, not character length        */
  uint8_t  bDescriptorType;     /**< USB_STRING_DESCRIPTOR                    */
  wchar_t  bString[1];          /**< UTF-16 string, no null termination       */
} usb_string_descriptor_t;

typedef struct usb_device   *usb_device_t;   /**< opaque  device  handle */
typedef struct usb_endpoint *usb_endpoint_t; /**< opaque endpoint handle */
typedef struct usb_transfer *usb_transfer_t; /**< opaque transfer handle */

#define USB_RETRY_FOREVER 0xFFFFFFu

#define usb_RootHub ((usb_device_t)0xD13FC0) /**< Root hub device */

/**
 * Type of the function to be called when a usb device event occurs.
 * @param device Handle for the device where the event originated.
 * @param data Opaque pointer passed to usb_Init().
 * @return Return USB_SUCCESS to initialize the device, USB_IGNORE to ignore it
 * without erroring, or an error to ignore the device and to return from
 * usb_ProcessEvents().
 */
typedef usb_error_t (*usb_device_event_callback_t)(usb_device_t device,
                                                   void *data,
                                                   usb_device_event_t event);

/**
 * Type of the function to be called when a transfer finishes.
 * @param device The device that was communicated with.
 * @param endpoint Endpoint communicated with, ored with hshb_index << 5.
 * @param status Status of the transfer.
 * @param data Opaque pointer passed to usb_ScheduleTransfer().
 * @param transferred The number of bytes transferred.
 * Only valid if \p status was USB_TRANSFER_COMPLETED.
 * @return Return USB_SUCCESS to free the transfer, USB_IGNORE to restart it,
 * or an error to free it and to return from usb_ProcessEvents().
 */
typedef usb_error_t (*usb_transfer_callback_t)(usb_device_t device,
                                               uint8_t endpoint,
                                               usb_transfer_status_t status,
                                               size_t transferred, void *data);

/**
 * Initializes the usb driver.
 * @param event_handler Function to be called when a usb event happens.
 * @param event_data Opaque pointer to be passed to the \p event_handler.
 * @param flags Which areas of memory to use.
 * @return USB_SUCCESS if initialization succeeded.
 * @note This must be called before any other function, and can be called again
 * to cancel all transfers and close all connections.
 */
usb_error_t usb_Init(usb_device_event_callback_t event_handler,
                     void *event_data, usb_init_flags_t flags);

/**
 * Uninitializes the usb driver.
 * @note This must be called before the program exits, or TIOS gets angry.
 */
void usb_Cleanup(void);

/**
 * Sets the descriptors to use when connected to a host.  If this is not called
 * before a call to usb_connect_callback_t with (flags & USB_CONNECTED_TO_HOST)
 * returns, or is called with NULL, then the default calculator device
 * descriptors will be used.  In a passed array, the first entry points to a
 * device descriptor, and the rest to each configuration descriptor.  If the
 * first entry is NULL, then that speed is disabled.  The arrays just need
 * to be readable, but the descriptors themselves must be in RAM.
 * @param descriptors An array of pointers to descriptors, pointer to NULL for
 * disabled, or NULL for default.
 */
void usb_SetDeviceDescriptors(void *const *full_speed_descriptors,
                              void *const *high_speed_descriptors);

/**
 * Calls any device or transfer callbacks that have triggered.
 * @return An error returned by a callback or USB_SUCCESS.
 */
usb_error_t usb_HandleEvents(void);

/**
 * Waits for any device or transfer events to occur, then calls their associated
 * callbacks.
 * @return An error returned by a callback or USB_SUCCESS.
 */
usb_error_t usb_WaitForEvents(void);

/**
 * Waits for any interrupt to occur, then calls any device or transfer callbacks
 * that may have triggered.
 * @return An error returned by a callback or USB_SUCCESS.
 */
usb_error_t usb_WaitForInterrupt(void);

/**
 * Gets the hub that \p device is attached to, or NULL if \p device is the root
 * hub.
 * @param device Device to get the hub of.
 * @return The hub device or NULL.
 */
usb_device_t usb_GetDeviceHub(usb_device_t device);

/**
 * Sets the user data associated with \p device.
 * @param device Device to set the user data of.
 */
void usb_SetDeviceUserData(usb_device_t device, void *data);

/**
 * Gets the user data associated with \p device.
 * @param device Device to get the user data of.
 * @return The user data associated with \p device.
 */
void *usb_GetDeviceUserData(usb_device_t device);

/**
 * Finds the next device connected through \p root after \p from satisfying
 * flags, or NULL if no more matching devices.
 * @param root Hub below which to limit search.
 * @param from Device to start the search from.
 * @param flags What kinds of devices to return.
 * @return The next matching device or NULL if none.
 */
usb_device_t usb_FindDevice(usb_device_t root, usb_device_t from,
                            usb_find_flag_t flags);

/**
 * Finds the first device satisfying flags.
 * @param flags What kinds of devices to return.
 * @return The first matching device, or NULL if no more matching devices.
 */
#define usb_FindFirstDevice(flags)\
  usb_FindDevice(usb_RootHub, usb_RootHub, flags)

/**
 * Finds the next device after \p from satisfying flags, or NULL if there are no
 * more devices that match.
 * @param from Device to start the search from.
 * @param flags What kinds of devices to return.
 * @return The next matching device or NULL if none.
 */
#define usb_FindNextDevice(from, flags)\
  usb_FindDevice(usb_RootHub, from, flags)

/**
 * Performs a usb reset on a device. This triggers a device enabled event when
 * the reset finishes.
 * @param device The device to reset.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_ResetDevice(usb_device_t device);

/**
 * Gets the usb address of a \p device, or 0 if disabled.
 * @param device The device to communicate with.
 * @return The usb address or 0.
 */
uint8_t usb_GetDeviceAddress(usb_device_t device);

/**
 * Gets the speed of a \device, or USB_SPEED_UNKNOWN if unknown.
 * @param device The device to communicate with.
 * @return The \c usb_speed_t.
 */
usb_speed_t usb_GetDeviceSpeed(usb_device_t device);

/**
 * Determines how large of a buffer would be required to receive the complete
 * configuration descriptor at \p index.
 * @note Blocks while the configuration descriptor is fetched.
 * @param device The device to communicate with.
 * @param index Which configuration descriptor to query.
 * @param total_length Returns the total number of bytes in a complete
 * configuration descriptor.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_GetConfigurationDescriptorTotalLength(usb_device_t device,
                                                      uint8_t index,
                                                      size_t *total_length);

/**
 * Gets the descriptor of a \p device of \p type at \p index.
 * @note Blocks while the descriptor is fetched.
 * @param device The device to communicate with.
 * @param type The \c usb_descriptor_type_t to fetch.
 * @param index Descriptor index to fetch.
 * @param descriptor Returns the fetched descriptor.
 * @param length The maximum number of bytes to receive.
 * The \p descriptor buffer must by at least this large.
 * @param transferred Returns the number of bytes actually received.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_GetDescriptor(usb_device_t device, usb_descriptor_type_t type,
                              uint8_t index, void *descriptor, size_t length,
                              size_t *transferred);

/**
 * Changes the descriptor at \p index.
 * @note Blocks while the descriptor is modified.
 * @note Devices do not usually support this.
 * @param device The device to communicate with.
 * @param type The \c usb_descriptor_type_t to change.
 * @param index The descriptor index to change..
 * @param descriptor The new descriptor.
 * @param length The number of bytes in the new descriptor.
 * The \p descriptor buffer must by at least this large.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_SetDescriptor(usb_device_t device, usb_descriptor_type_t type, uint8_t index,
                              const void *descriptor, size_t length);

/**
 * Gets the string descriptor at \p index.
 * @note Blocks while the descriptor is fetched.
 * @param device The device to communicate with.
 * @param type Descriptor type to fetch.
 * @param index Descriptor index to fetch.
 * @param descriptor Returns the fetched descriptor.
 * @param length The number of bytes to transfer.
 * The \p descriptor buffer must by at least this large.
 * @param transferred Returns the number of bytes actually received.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_GetStringDescriptor(usb_device_t device, uint8_t type, uint8_t index,
                                    void *descriptor, size_t length,
                                    size_t *transferred);

/**
 * Changes the descriptor at \p index, not usually supported.  Blocks while
 * the descriptor is modified.
 * @note Devices do not usually support this.
 * @param device The device to communicate with.
 * @param type Descriptor type to modify.
 * @param index Descriptor index to modify.
 * @param descriptor The new descriptor data.
 * @param length The number of bytes in the new descriptor.
 * The \p descriptor buffer must by at least this large.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_SetStringDescriptor(usb_device_t device, uint8_t type, uint8_t index,
                                    const void *descriptor, size_t length);

/**
 * Gets the currently active configuration of a device.
 * @param device The device to communicate with.
 * @param index Returns the current configuration value, or 0 if unconfigured.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_GetConfiguration(usb_device_t device, uint8_t *index);

/**
 * Selects the configuration specified by the \p configuration_descriptor.
 * This must be called before pipes other than the default control pipe can be
 * accessed.
 * @param device The device to communicate with.
 * @param configuration_descriptor A complete configuration descriptor fetched
 * with usb_GetDescriptor().
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_SetConfiguration(usb_device_t device,
                                 const void *configuration_descriptor);

/**
 * Gets the current alternate setting in use on the specified interface.
 * @param device The device to communicate with.
 * @param interface Interface index to query.
 * @param alternate_setting Returns the alternate setting in use.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_GetInterfaceAltSetting(usb_device_t device, uint8_t interface,
                                       uint8_t *alternate_setting);

/**
 * Sets the alternate setting in use on the specified interface.
 * @param device The device to communicate with.
 * @param interface Interface index to modify.
 * @param alternate_setting Alternate setting to use.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_SetInterfaceAltSetting(usb_device_t device, uint8_t interface,
                                       uint8_t alternate_setting);

/**
 * Gets the endpoint of a \p device with a given \p address, or NULL if that
 * address is unused.
 * @param device Device to get the user data of.
 * @param address Address of the endpoint to get.
 * @return The specified endpoint or NULL.
 */
usb_endpoint_t usb_GetDeviceEndpoint(usb_device_t device, uint8_t address);

/**
 * Gets the device that \p endpoint is connected to.
 * @param endpoint Endpoint to get the device of.
 * @return The device for an \p endpoint.
 */
usb_device_t usb_GetEndpointDevice(usb_endpoint_t endpoint);

/**
 * Gets the maximum packet size of an endpoint.
 * @param endpoint The endpoint to get the maximum packet size of..
 * @return The wMaxPacketSize for an \p endpoint.
 */
uint16_t usb_GetEndpointMaxPacketSize(usb_endpoint_t endpoint);

/**
 * Gets the transfer type of an endpoint.
 * @param endpoint The endpoint to get the transfer type of.
 * @return The usb_transfer_type for an endpoint.
 */
usb_transfer_type_t usb_GetEndpointTransferType(usb_endpoint_t endpoint);

/**
 * Clears an endpoint's halt condition, indicated by transfers to that endpoint
 * stalling.  This function blocks until the halt condition is cleared.
 * @param endpoint The endpoint to clear the halt condition of.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_ClearEndpointHalt(usb_endpoint_t endpoint);

/**
 * Schedules a transfer to the pipe connected to \p endpoint of \p device, in
 * the direction indicated by \p setup->bmRequestType, using \p buffer as the
 * data buffer, \p setup->wLength as the buffer length, and then waits for it to
 * complete.  If acting as usb host and using a control pipe, \p setup is used
 * as the setup packet, otherwise all fields not mentioned above are ignored.
 * @param device The device to communicate with.
 * @param endpoint Address of the endpoint to communicate with. Bit 7 is
 * ignored.
 * @param setup Indicates the transfer direction and buffer length.  If acting
 * as usb host and using a control pipe, also used as the setup packet to send.
 * @param buffer Data to transfer that must reside in RAM and have room for at
 * least \p setup->wLength bytes.
 * @param retries How many times to retry the transfer before timing out.
 * If retries is USB_RETRY_FOREVER, the transfer never times out.
 * @param transferred Returns the number of bytes actually transferred.
 * If \p transferred is NULL then nothing is returned.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_ControlTransfer(usb_endpoint_t endpoint,
                                usb_control_setup_t *setup, void *buffer,
                                unsigned retries, size_t *transferred);

/**
 * Schedules a control transfer to the default control pipe of \p device, in the
 * direction indicated by \p setup->bmRequestType, using \p buffer as the data
 * buffer, \p setup->wLength as the buffer length, and then waits for it to
 * complete.  If acting as usb host, \p setup is used as the setup packet,
 * otherwise all fields not mentioned above are ignored.
 * @param device The device to communicate with.
 * @param setup Indicates the transfer direction and buffer length.  If acting
 * as usb host, also used as the setup packet to send.
 * @param buffer Data to transfer that must reside in RAM and be at least
 * setup->wLength bytes.
 * @param retries How many times to retry the transfer before timing out.
 * If retries is USB_RETRY_FOREVER, the transfer never times out.
 * @param transferred Returns the number of bytes actually transferred.
 * NULL means don't return anything.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
#define usb_DefaultControlTransfer(device, setup, buffer, retries, transferred)\
  usb_ControlTransfer(device, 0, setup, buffer, retries, transferred)

/**
 * Schedules a transfer to the pipe connected to \p endpoint of \p device, using
 * \p length as the buffer length, and waits for it to complete. If acting as
 * usb host and using a control pipe, uses the beginning of \p buffer as the
 * setup packet to send, its \c bmRequestType for the transfer direction, and
 * the rest of \p buffer as the data buffer.  Otherwise, uses \p endpoint for
 * transfer direction and  the whole \p buffer as the data buffer.
 * @param device The device to communicate with.
 * @param endpoint Address of the endpoint to communicate with. Bit 7 indicates
 * the transfer direction for non-control transfers.
 * @param buffer Data to transfer that must reside in RAM.  When acting as usb
 * host and using a control pipe, starts with a \c usb_control_setup_t.
 * @param length Number of bytes to transfer.  The \p buffer must be at least
 * this large.  However, this is ignored for control transfers when acting as
 * usb host where the \c wLength of the setup packet is used instead.
 * @param retries How many times to retry the transfer before timing out.
 * If retries is USB_RETRY_FOREVER, the transfer never times out.
 * @param transferred Returns the number of bytes actually transferred.
 * NULL means don't return anything.
 * @return USB_SUCCESS if the transfer succeeded or an error.
 */
usb_error_t usb_Transfer(usb_device_t device, uint8_t endpoint, void *buffer,
                         size_t length, unsigned retries, size_t *transferred);
#define usb_BulkTransfer usb_Transfer
#define usb_InterruptTransfer usb_Transfer
#define usb_IsochronousTransfer usb_Transfer

/**
 * Schedules a transfer to the pipe connected to \p endpoint of \p device, in
 * the direction indicated by \p setup->bmRequestType, using \p buffer as the
 * data buffer, and \p setup->wLength as the buffer length.  If acting as usb
 * host and using a control pipe, \p setup is used as the setup packet,
 * otherwise all fields not mentioned above are ignored.
 * @param device The device to communicate with.
 * @param endpoint Address of endpoint to communicate with.  Bit 7 indicates the
 * transfer direction for non-control transfers.
 * @param setup Indicates the transfer direction and buffer length.  If acting
 * as usb host and using a control pipe, also used as the setup packet to send.
 * @param buffer Data to transfer that must reside in RAM.  This buffer must
 * remain valid until the callback is called i.e. it cannot be modified or
 * freed.
 * @param transferred Returns the number of bytes actually transferred.
 * @param handler Function to be called when the transfer finishes.
 * @param data Opaque pointer to be passed to the \p handler.
 * @return USB_SUCCESS if the transfer was scheduled or an error.
 */
usb_error_t usb_ScheduleControlTransfer(usb_device_t device, uint8_t endpoint,
                                        usb_control_setup_t *setup,
                                        void *buffer,
                                        usb_transfer_callback_t handler,
                                        void *data);

/**
 * Schedules a control transfer to the default control pipe of \p device, in
 * the direction indicated by \p setup->bmRequestType, using \p buffer as the
 * data buffer, and \p setup->wLength as the buffer length.  If acting as usb
 * host, \p setup is used as the setup packet, otherwise all fields not
 * mentioned above are ignored.
 * @param device The device to communicate with.
 * @param setup Indicates the transfer direction and buffer length.  If acting
 * as usb host, also used as the setup packet to send.
 * @param buffer Data to transfer that must reside in RAM.  This buffer must
 * remain valid until the callback is called i.e. it cannot be modified or
 * freed.
 * @param transferred Returns the number of bytes actually transferred.
 * @param handler Function to be called when the transfer finishes.
 * @param data Opaque pointer to be passed to the \p handler.
 * @return USB_SUCCESS if the transfer was scheduled or an error.
 */
#define usb_ScheduleDefaultControlTransfer(device, setup, buffer, handler, data)\
  usb_ScheduleControlTransfer(device, 0, setup, buffer, handler, data)

/**
 * If endpoint is not a control endpoint, schedules a transfer of the endpoint's
 * transfer type.  If the endpoint is a control endpoint, schedules a control
 * transfer interpreting the beginning of buffer as the \c usb_control_setup_t
 * and using the rest of the buffer as the transfer buffer.
 * @param device The device to communicate with.
 * @param endpoint Address of endpoint to communicate with.
 * Bit 7 is ignored for control transfers, and the direction of other transfers.
 * @param type Type of the endpoint and transfer.
 * @param buffer Data to transfer that must reside in RAM.  When acting as usb
 * host and using a control pipe, starts with a \c usb_control_setup_t.  This
 * buffer must remain valid until the callback is called i.e. it cannot be
 * modified or freed.
 * @param length Number of bytes to transfer.  The \p buffer must be at least
 * this large.  However, this is ignored for control transfers when acting as
 * usb host where the \c wLength of the setup packet is used instead.
 * @param transferred Returns the number of bytes actually transferred.
 * @param handler Function to be called when the transfer finishes.
 * @param data Opaque pointer to be passed to the \p handler.
 * @return USB_SUCCESS if the transfer was scheduled or an error.
 */
usb_error_t usb_ScheduleTransfer(usb_device_t device, uint8_t endpoint,
                                 void *buffer, size_t length,
                                 usb_transfer_callback_t handler, void *data);
#define usb_ScheduleBulkTransfer usb_ScheduleTransfer
#define usb_ScheduleInterruptTransfer usb_ScheduleTransfer
#define usb_ScheduleIsochronousTransfer usb_ScheduleTransfer

#ifdef __cplusplus
}
#endif

#endif