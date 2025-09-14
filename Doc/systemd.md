# UltraNote Infinity Systemd Service Setup

## Overview

Systemd is the recommended way to ensure your UltraNote Infinity infrastructure keeps running reliably after reboots or failures. This guide covers setting up systemd services for both the node daemon (`ultranoteid`) and wallet daemon (`walletd`).

## Prerequisites

- UltraNote Infinity binaries compiled and ready for deployment
- Systemd-enabled Linux distribution (Ubuntu, Debian, CentOS, etc.)
- Root or sudo access for service installation
- Properly configured node and wallet configuration files

## Node Daemon Service (`ultranoteid`)

### Service File Setup

1. **Create the service file** at `/etc/systemd/system/ultranoteid.service`:

```ini
[Unit]
Description=UltraNote Infinity Node Daemon
After=network.target
Wants=network.target

[Service]
Type=simple
User=ultranote
Group=ultranote
ExecStart=/usr/bin/ultranoteid --config-file /etc/XUNI/node.conf
Restart=always
RestartSec=30
StandardOutput=syslog
StandardError=syslog
SyslogIdentifier=ultranoteid
LimitNOFILE=65536

# Security hardening
NoNewPrivileges=yes
PrivateTmp=yes
ProtectSystem=strict
ProtectHome=yes

[Install]
WantedBy=multi-user.target
```

2. **Copy the binary** to system location:
```bash
sudo cp build/release/src/ultranoteid /usr/bin/ultranoteid
sudo chmod +x /usr/bin/ultranoteid
```

3. **Create system user** (optional but recommended):
```bash
sudo useradd -r -s /bin/false ultranote
```

### Configuration

Ensure your node configuration file `/etc/XUNI/node.conf` contains appropriate settings:

```ini
data-dir=/var/lib/ultranote
log-file=/var/log/ultranoteid.log
log-level=1
p2p-bind-ip=0.0.0.0
p2p-bind-port=42001
rpc-bind-ip=127.0.0.1
rpc-bind-port=43000
```

### Service Management

**Start the service:**
```bash
sudo systemctl start ultranoteid
```

**Check status:**
```bash
sudo systemctl status ultranoteid
```

**Enable auto-start:**
```bash
sudo systemctl enable ultranoteid
```

**Common commands:**
```bash
sudo systemctl stop ultranoteid      # Stop service
sudo systemctl restart ultranoteid   # Restart service
sudo systemctl reload ultranoteid    # Reload configuration
sudo journalctl -u ultranoteid -f   # View logs in real-time
```

## Wallet Daemon Service (`walletd`)

### Service File Setup

1. **Create the service file** at `/etc/systemd/system/walletd.service`:

```ini
[Unit]
Description=UltraNote Infinity Wallet Daemon
After=network.target ultranoteid.service
Wants=network.target

[Service]
Type=simple
User=ultranote
Group=ultranote
ExecStart=/usr/bin/walletd --config-file /etc/XUNI/paymentgate.conf
Restart=always
RestartSec=30
StandardOutput=syslog
StandardError=syslog
SyslogIdentifier=walletd
LimitNOFILE=65536

# Security hardening
NoNewPrivileges=yes
PrivateTmp=yes
ProtectSystem=strict
ProtectHome=yes

[Install]
WantedBy=multi-user.target
```

2. **Copy the binary** to system location:
```bash
sudo cp build/release/src/walletd /usr/bin/walletd
sudo chmod +x /usr/bin/walletd
```

### Configuration

Ensure your wallet configuration file `/etc/XUNI/paymentgate.conf` contains appropriate settings:

```ini
container-file=/var/lib/ultranote/container.iwallet
container-password=your_secure_password_here
data-dir=/var/lib/ultranote
bind-address=127.0.0.1
bind-port=8070
rpc-user=your_rpc_username
rpc-password=your_rpc_password
log-file=/var/log/walletd.log
log-level=1
local=1
```

### Service Management

**Start the service:**
```bash
sudo systemctl start walletd
```

**Check status:**
```bash
sudo systemctl status walletd
```

**Enable auto-start:**
```bash
sudo systemctl enable walletd
```

## Advanced Configuration

### Remote Node Setup

For running as a remote-accessible node:

```ini
ExecStart=/usr/bin/ultranoteid \
  --data-dir=/var/lib/ultranote \
  --rpc-bind-ip=0.0.0.0 \
  --rpc-bind-port=33000 \
  --enable-cors=* \
  --enable-blockchain-indexes
```

### Resource Limits

Adjust resource limits in service files for optimal performance:

```ini
# Memory limits (adjust based on available RAM)
MemoryMax=2G
MemoryHigh=1.5G

# CPU limits
CPUQuota=200%
```

### Logging Configuration

Configure log rotation by creating `/etc/logrotate.d/ultranote`:

```
/var/log/ultranoteid.log
/var/log/walletd.log {
    daily
    rotate 7
    compress
    delaycompress
    missingok
    notifempty
    create 640 ultranote ultranote
}
```

## Security Considerations

### File Permissions

Set proper permissions for security:

```bash
sudo chown -R ultranote:ultranote /var/lib/ultranote
sudo chmod 700 /var/lib/ultranote
sudo chmod 600 /etc/XUNI/*.conf
```

### Firewall Configuration

Configure firewall for necessary ports only:

```bash
# Allow P2P communication
sudo ufw allow 42001/tcp

# Allow RPC access (restrict to specific IPs if needed)
sudo ufw allow from 192.168.1.0/24 to any port 43000
sudo ufw allow from 192.168.1.0/24 to any port 8070
```

### SSL/TLS Configuration

For secure RPC communication, consider using a reverse proxy with SSL:

```nginx
# Nginx configuration example
server {
    listen 443 ssl;
    server_name your-domain.com;
    
    ssl_certificate /path/to/cert.pem;
    ssl_certificate_key /path/to/key.pem;
    
    location /json_rpc {
        proxy_pass http://127.0.0.1:8070;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

## Troubleshooting

### Common Issues

1. **Permission denied errors:**
   ```bash
   sudo chown -R ultranote:ultranote /var/lib/ultranote
   ```

2. **Port already in use:**
   Check for other processes using the same ports and adjust configuration.

3. **Synchronization issues:**
   Ensure sufficient disk space and check network connectivity.

4. **Wallet container errors:**
   Verify container file permissions and password correctness.

### Log Analysis

Use journalctl to analyze logs:

```bash
# View last 100 lines of ultranoteid logs
sudo journalctl -u ultranoteid -n 100

# Follow logs in real-time
sudo journalctl -u ultranoteid -f

# View logs since last boot
sudo journalctl -u ultranoteid --since boot

# Filter for error messages
sudo journalctl -u ultranoteid -p err
```

### Performance Monitoring

Monitor system resources:

```bash
# Check CPU and memory usage
top -p $(pgrep ultranoteid) -p $(pgrep walletd)

# Check disk I/O
iotop -p $(pgrep ultranoteid) -p $(pgrep walletd)

# Check network connections
ss -tulpn | grep -E '(ultranoteid|walletd)'
```

## Maintenance

### Regular Maintenance Tasks

1. **Backup wallet containers** regularly
2. **Monitor disk space** for blockchain growth
3. **Update software** when new versions are released
4. **Review logs** for any anomalies or errors
5. **Test restart procedures** to ensure reliability

### Update Procedure

When updating to new versions:

```bash
# Stop services
sudo systemctl stop ultranoteid walletd

# Backup existing installation
sudo cp /usr/bin/ultranoteid /usr/bin/ultranoteid.backup
sudo cp /usr/bin/walletd /usr/bin/walletd.backup

# Install new binaries
sudo cp new-build/ultranoteid /usr/bin/ultranoteid
sudo cp new-build/walletd /usr/bin/walletd

# Start services
sudo systemctl start ultranoteid walletd
```

## Support

For additional assistance with systemd configuration:
- Check the main [README.md](../README.md) for general guidance
- Review service files in the `Doc/` directory for examples
- Consult systemd documentation for advanced configuration options
- Check GitHub issues for common problems and solutions
