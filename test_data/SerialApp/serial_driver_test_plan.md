<!--
  TestForge™ — SerialApp Training Example Test Plan
  © 2026 Myattech. All rights reserved.

  This is a pre-annotated plan demonstrating the DCE workflow.
  STEP 1: Review this plan (Discovery is already done)
  STEP 2: Modify constraints to match your project (Curation)
  STEP 3: Run: testforge run --plan test_data/SerialApp/serial_driver_test_plan.md

  M12 mock_valid_struct:
    - "mock_valid_struct": true on a pointer means the live runner (PlanRunner)
      will allocate a real ctypes struct or buffer (instead of NULL or fake ptr).
    - For "handle" it builds a SerialHandle with baud_rate=115200 etc.
    - For data/out_buf it builds a byte buffer with test pattern.
    - Post-call state (rx_buffer contents, rx_count, etc.) is captured into
      the ExecutionReport / TestResult for inspection in text + HTML reports.
    - Extended plan JSON can include "struct_fields" + "buffer_size" for
      arbitrary structs (see PointerConstraint in constraints.py).
-->

# TestForge™ Test Plan: serial_driver.c

**Source:** `test_data/SerialApp/serial_driver.c`
**Workflow:** Discovery → Curation → Execution (DCE)
**Backend:** mock (change to "serial" for real loopback)

---

## Functions Under Test

### serial_init

**Description:** Initializes the serial handle with baud rate and port.
**Guard detected:** baud_rate == 0 returns SERIAL_ERR_NULL

```json
{
  "function": "serial_init",
  "source": "test_data/SerialApp/serial_driver.c",
  "stress_mode": false,
  "mock_enabled": false,
  "comm_backend": "mock",
  "expected_exception": null,
  "description": "Init with valid and invalid baud rates",
  "notes": "Set stress_mode=true to test baud_rate=4294967295",
  "parameters": [
    {
      "param_name": "handle",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true,
      "struct_fields": [
        {"name": "baud_rate", "ctype": "uint32"},
        {"name": "port_id", "ctype": "uint8"},
        {"name": "initialized", "ctype": "uint8"},
        {"name": "rx_buffer", "ctype": "array"},
        {"name": "rx_count", "ctype": "uint16"}
      ]
    },
    {
      "param_name": "baud_rate",
      "type": "integer",
      "min": 1,
      "max": 4000000,
      "custom_values": [0, 9600, 115200, 921600],
      "exclude_values": []
    },
    {
      "param_name": "port_id",
      "type": "integer",
      "min": 0,
      "max": 7,
      "custom_values": [0, 1, 7],
      "exclude_values": []
    }
  ]
}
```

### serial_send

**Description:** Sends raw bytes — validates handle, data, and length.
**Guards detected:** handle==NULL, data==NULL, length==0, length>256

```json
{
  "function": "serial_send",
  "source": "test_data/SerialApp/serial_driver.c",
  "stress_mode": true,
  "mock_enabled": true,
  "comm_backend": "mock",
  "expected_exception": null,
  "description": "Send with boundary lengths — stress tests overflow",
  "notes": "stress_mode=true adds length=257 (overflow) test case",
  "parameters": [
    {
      "param_name": "handle",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    },
    {
      "param_name": "data",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    },
    {
      "param_name": "length",
      "type": "integer",
      "min": 0,
      "max": 256,
      "custom_values": [0, 1, 128, 255, 256],
      "exclude_values": []
    }
  ]
}
```

### serial_receive

**Description:** Receives bytes into caller buffer.
**Guards detected:** NULL guards on handle/out_buf/out_length

```json
{
  "function": "serial_receive",
  "source": "test_data/SerialApp/serial_driver.c",
  "stress_mode": false,
  "mock_enabled": true,
  "comm_backend": "mock",
  "expected_exception": null,
  "description": "Receive into buffer with size boundaries",
  "notes": "",
  "parameters": [
    {
      "param_name": "handle",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    },
    {
      "param_name": "out_buf",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    },
    {
      "param_name": "buf_size",
      "type": "integer",
      "min": 0,
      "max": 256,
      "custom_values": [0, 16, 256],
      "exclude_values": []
    },
    {
      "param_name": "out_length",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    }
  ]
}
```

### serial_compute_crc

**Description:** XOR CRC over byte array — pure function, no HAL.
**Guards detected:** None (pure computation)

```json
{
  "function": "serial_compute_crc",
  "source": "test_data/SerialApp/serial_driver.c",
  "stress_mode": false,
  "mock_enabled": false,
  "comm_backend": "mock",
  "expected_exception": null,
  "description": "CRC over empty, single-byte, and known test vectors",
  "notes": "Verify CRC([0xAA, 0x01, 0xFF]) == expected XOR result",
  "parameters": [
    {
      "param_name": "data",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    },
    {
      "param_name": "length",
      "type": "integer",
      "min": 0,
      "max": 256,
      "custom_values": [0, 1, 255],
      "exclude_values": []
    }
  ]
}
```

### serial_send_framed

**Description:** Framed send (START + payload + CRC + END).
**Guards detected:** from serial_send + frame size

```json
{
  "function": "serial_send_framed",
  "source": "test_data/SerialApp/serial_driver.c",
  "stress_mode": true,
  "mock_enabled": true,
  "comm_backend": "mock",
  "expected_exception": null,
  "description": "Framed send with length boundaries (stress includes overflow)",
  "notes": "",
  "parameters": [
    {
      "param_name": "handle",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    },
    {
      "param_name": "payload",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    },
    {
      "param_name": "length",
      "type": "integer",
      "min": 0,
      "max": 253,
      "custom_values": [0, 1, 128, 253, 254],
      "exclude_values": []
    }
  ]
}
```

### serial_receive_framed

**Description:** Framed receive with validation of start/end/CRC.

```json
{
  "function": "serial_receive_framed",
  "source": "test_data/SerialApp/serial_driver.c",
  "stress_mode": false,
  "mock_enabled": true,
  "comm_backend": "mock",
  "expected_exception": null,
  "description": "Framed receive — happy path + bad frame/CRC cases",
  "notes": "",
  "parameters": [
    {
      "param_name": "handle",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    },
    {
      "param_name": "out_payload",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    },
    {
      "param_name": "buf_size",
      "type": "integer",
      "min": 0,
      "max": 256,
      "custom_values": [16, 256],
      "exclude_values": []
    },
    {
      "param_name": "out_length",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    }
  ]
}
```

### serial_reset

**Description:** Zero the handle.

```json
{
  "function": "serial_reset",
  "source": "test_data/SerialApp/serial_driver.c",
  "stress_mode": false,
  "mock_enabled": false,
  "comm_backend": "mock",
  "expected_exception": null,
  "description": "Reset handle (NULL guard)",
  "notes": "",
  "parameters": [
    {
      "param_name": "handle",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    }
  ]
}
```

### serial_is_initialized

**Description:** Return whether handle has been initialized.

```json
{
  "function": "serial_is_initialized",
  "source": "test_data/SerialApp/serial_driver.c",
  "stress_mode": false,
  "mock_enabled": false,
  "comm_backend": "mock",
  "expected_exception": null,
  "description": "Query initialized flag (NULL guard)",
  "notes": "",
  "parameters": [
    {
      "param_name": "handle",
      "type": "pointer",
      "allow_null": true,
      "mock_valid_struct": true
    }
  ]
}
```
