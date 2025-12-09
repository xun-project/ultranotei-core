# UltraNote DNS (OpenAlias) Integration Guide

## Overview
UltraNote supports the **OpenAlias** standard (prefix `oa1`), allowing users to send funds to human-readable domain names (e.g., `me.ultranote.org`) instead of long, complex wallet addresses. This document outlines how the system works and how to configure it.

## How It Works in Core
The integration handles address resolution transparently:
1.  **Input Detection**: When a user enters a destination in the wallet, the system first checks if it's a valid Base58 address.
2.  **DNS Lookup**: If the input is not a raw address, the wallet performs a DNS TXT record lookup for the domain.
3.  **Parsing**: The wallet parses the returned TXT records looking for the OpenAlias standard format:
    *   Finds prefix: `oa1:xuni`
    *   Extracts value: `recipient_address=<address>`

## Configuration Guide for Users/Admins

To enable an alias for your wallet, you must configure a DNS TXT record on your domain. No code changes are required in the wallet software.

### DNS Record Format
The TXT record must follow this specific format:
```text
oa1:xuni recipient_address=<YOUR_WALLET_ADDRESS>; recipient_name=<OPTIONAL_NAME>;
```

### Critical Requirements
*   **Prefix**: Must start with `oa1:xuni`.
*   **Address Length**: The wallet expects a valid UltraNote address of **99 characters**.
*   **Terminator**: The address field **must** be followed by a semicolon `;`. The parser relies on this to determine where the address string ends.

### Example Configuration
For the domain `desp.ultranote.org`, the following record is used:

| Type | Host | Value |
|------|------|-------|
| TXT  | `desp` | `oa1:xuni recipient_address=Xuniik72MxR2Cn29BzvVcsPmPn2NxNPGHfpYocW7CrjhLVs928LUmgfBJnVJE1xoHy8fdL3dVcLzF49J9Y8SsryHYMWMX4BySs3; recipient_name=Desp.` |

### Verification
You can verify your DNS record setup using command line tools like `nslookup` or `dig`:

**Windows:**
```powershell
nslookup -q=TXT desp.ultranote.org
```

**Linux/macOS:**
```bash
dig +short TXT desp.ultranote.org
```

## Developer Notes
*   **Code Location**: `src/UltraNoteWallet/UltraNoteWallet.cpp` (Parsing logic) and `src/Common/DnsTools.cpp` (Network lookup).
*   **Testing**: A mock response is implemented in `DnsTools.cpp` for the domain `desp.ultranote.org` to facilitate strictly local testing without network propagation.
