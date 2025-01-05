# UltraNote Infinity Systemd Guide

## Overview
This guide covers systemd service configuration for UltraNote Infinity components:
- `ultranoteid` - Core daemon
- `ultranotei-walletd` - Wallet service
- `walletd` - Payment gateway service

## Installation

### 1. Prepare System
```bash
sudo apt update
sudo apt install curl jq
```

### 2. Create System User
```bash
sudo useradd -r -s /bin/false ultranotei
sudo mkdir -p /var/lib/ultranotei
sudo chown ultranotei:ultranotei /var/lib/ultranotei
```

### 3. Install Binaries
```bash
sudo cp ultranoteid /usr/local/bin/
sudo cp ultranotei-walletd /usr/local/bin/
sudo cp walletd /usr/local/bin/
sudo chmod +x /usr/local/bin/ultranoteid /usr/local/bin/ultranotei-walletd /usr/local/bin/walletd
```

## Service Configuration

### Core Daemon (ultranoteid.service)
```ini
[Unit]
Description=UltraNote Infinity Core Daemon
After=network.target

[Service]
User=ultranotei
Group=ultranotei
ExecStart=/usr/local/bin/ultranoteid --data-dir=/var/lib/ultranotei
Restart=always
RestartSec=30
ProtectSystem=full
NoNewPrivileges=true
PrivateTmp=true
MemoryDenyWriteExecute=true

[Install]
WantedBy=multi-user.target
```

### Wallet Service (ultranotei-walletd.service)
```ini
[Unit]
Description=UltraNote Infinity Wallet Service
After=ultranoteid.service

[Service]
User=ultranotei
Group=ultranotei
ExecStart=/usr/local/bin/ultranotei-walletd \
  --data-dir=/var/lib/ultranotei \
  --rpc-user=YOUR_USER \
  --rpc-password=YOUR_PASSWORD \
  --wallet-file=/var/lib/ultranotei/wallet \
  --password=WALLET_PASSWORD
Restart=always
RestartSec=30
ProtectSystem=full
NoNewPrivileges=true
PrivateTmp=true
MemoryDenyWriteExecute=true

[Install]
WantedBy=multi-user.target
```

## Security Best Practices

1. Use strong passwords for RPC and wallet
2. Restrict RPC access to localhost
3. Use firewall rules to limit access
4. Regularly update binaries
5. Monitor service logs

## Troubleshooting

### Common Issues

1. **Service fails to start**
   - Check logs: `journalctl -u ultranoteid`
   - Verify permissions: `sudo chown -R ultranotei:ultranotei /var/lib/ultranotei`

2. **RPC not accessible**
   - Verify service is running: `systemctl status ultranoteid`
   - Check firewall rules
   - Verify RPC credentials

3. **Synchronization issues**
   - Check peer count: `curl -u user:pass -d '{"jsonrpc":"2.0","id":"1","method":"getStatus"}' http://127.0.0.1:8070/json_rpc`
   - Verify network connectivity

## Maintenance

### Backup Wallet
```bash
sudo systemctl stop ultranotei-walletd
cp /var/lib/ultranotei/wallet /backup/location/
sudo systemctl start ultranotei-walletd
```

### Update Binaries
1. Stop services
2. Backup configuration
3. Install new binaries
4. Restart services
