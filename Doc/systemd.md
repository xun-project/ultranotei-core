## UltraNote Infinity Systemctl Daemon

Make sure your UltraNote Infinity infrastructure keeps running after a reboot or after any failure. For this we recommand systemd

Make sure you have the binaries downloaded in advance.

To use the UltraNote Infinity Daemon Service, update the `ultranoteid.service` file.
Next, copy the `ultranoted.service` file to `/etc/systemd/system/ultranoteid.service`.
After that, copy the `ultranoteid` binary to `/usr/bin/ultranoteid`.

Now test the setup with `sudo systemctl start ultranoteid`.
To check the status, run `sudo systemctl status ultranoteid`.
Once it works, run `sudo systemctl enable ultranoteid` to make sure ultranoteid starts at boot and after a failure.

Restarting: `sudo systemctl restart ultranoteid`
Stopping: `sudo systemctl stop ultranoteid`
Starting: `sudo systemctl start ultranoteid`

*Same process goes for `ultranotei-walletd.service`*

### Running as a remote node use

`ExecStart=/usr/bin/ultranoteid --data-dir=/home/YOUR_USER/.ultranotei --rpc-bind-ip=0.0.0.0 --rpc-bind-port=33000`
