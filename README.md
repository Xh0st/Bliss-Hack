Bliss Exploit – Xbox One Fat (Model 1540) Hardware Glitch + eMMC Injection

A two‑stage voltage glitch attack on the Xbox One Fat’s NB_CORE rail, followed by parallel payload injection over the eMMC bus to disable hypervisor signature checks.

This repository provides a complete, from‑scratch implementation of the Bliss Exploit, as demonstrated at RE//verse 2026 and verified on dashboard 10.0.26100.7010 (xb_flt_2602ge.260212-1010).

---

✨ Features

· Hardware‑only – no software vulnerability required
· Two‑stage glitch – MPU bypass + instruction substitution
· Parallel eMMC injection – 8‑bit bit‑bang for fast payload delivery
· Hypervisor patch – NOPs the signature verification routine
· Post‑exploit – dump bootrom, hypervisor, and eventually HDD key

---

🔧 Hardware Required

· Xbox One Fat (Model 1540)
· Teensy 4.1
· IRLU024N MOSFET
· TXS0108E level shifter (1.8V ↔ 3.3V)
· 0.1Ω shunt resistor
· Fine wire, soldering equipment

---

📦 What's Inside

· ARM64 payload assembly – prints heartbeat, patches hypervisor, dumps bootrom
· Teensy 4.1 Arduino sketch – precise glitch timing + parallel injection
· Python listener – captures UART output from the Teensy
· Detailed wiring guide – test point locations, MOSFET wiring, level shifter connections
· Post‑exploit instructions – dumping hypervisor and finding the correct signature‑check offset

---

🚀 Quick Start

1. Solder the glitch circuit and eMMC injection bus (level shifter required).
2. Compile the ARM64 payload and convert it to a C array.
3. Upload the Teensy sketch with the payload embedded.
4. Run the Python listener and power on the Xbox.
5. Wait for the !!! BLISS_WIN: PAYLOAD INJECTED !!! message and bootrom dump.

---

⚠️ Disclaimer

This project is for educational and security research purposes only. Modifying your console may void the warranty and violate the Xbox Terms of Service. Use at your own risk.

---

📚 Further Reading

· RE//verse 2026 – The Bliss Exploit
· Xbox One Fat motherboard schematics (for test point locations)
· TXS0108E datasheet

---

🤝 Contributing

Issues and pull requests are welcome – especially for hypervisor offsets on other dashboard versions.

---

Star ⭐ this repo if you find it useful!
