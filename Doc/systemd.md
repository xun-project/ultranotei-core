# UltraNote Infinity Systemd Service Management

## Overview
This document provides comprehensive documentation for managing UltraNote Infinity services using systemd.

## Service Configuration

### Required Files
- `ultranoteid.service`
- `ultranotei-walletd.service`
- `walletd.service`

### Installation Steps
1. Copy service files to systemd directory:
```bash
sudo cp ultranoteid.service /etc/systemd/system/
sudo cp ultranotei-walletd.service /etc/systemd/system/
sudo cp walletd.service /etc/systemd/system/
```

2. Copy binaries to system path:
```bash
sudo cp ultranoteid /usr/bin/
sudo cp ultranotei-walletd /usr/bin/
sudo cp walletd /usr/bin/
```

3. Reload systemd configuration:
```bash
sudo systemctl daemon-reload
```

## Service Management

### Starting Services
```bash
sudo systemctl start ultranoteid
sudo systemctl start ultranotei-walletd
sudo systemctl start walletd
```

### Enabling Services
```bash
sudo systemctl enable ultranoteid
sudo systemctl enable ultranotei-walletd
sudo systemctl enable walletd
```

### Checking Status
```bash
sudo systemctl status ultranoteid
sudo systemctl status ultranotei-walletd
sudo systemctl status walletd
```

## Configuration Examples

### Local Node Configuration
```ini
ExecStart=/usr/bin/ultranoteid --data-dir=/home/YOUR_USER/.ultranotei
```

### Remote Node Configuration
```ini
ExecStart=/usr/bin/ultranoteid --data-dir=/home/YOUR_USER/.ultranotei \
  --rpc-bind-ip=0.0.0.0 \
  --rpc-bind-port=33000
```

## Troubleshooting

### Common Issues
- Service fails to start
- Permission denied errors
- Port conflicts
- Resource allocation problems

### Diagnostic Commands
```bash
# Check service logs
journalctl -u ultranoteid

# Verify file permissions
ls -l /usr/bin/ultranoteid

# Check port usage
sudo netstat -tulpn | grep 33000
```

## Support
For technical assistance, contact:
- Email: support@ultranote.org
- GitHub Issues: [Report Issues](https://github.com/xun-project/UltraNote-RPC-PHP/issues)
