function isDeviceRawHidInterface(device) {
  return device.collections.some(
    (col) => col.usagePage === 0xff00 && col.usage === 0x01
  );
}

function delayMs(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

async function start() {
  const devices = (
    await navigator.hid.requestDevice({
      filters: [{ vendorId: 0xf055, productId: 0xa57a }],
    })
  ).filter(isDeviceRawHidInterface);

  console.log({ devices });

  const device = devices[0];
  if (device) {
    console.log(device);
    console.log(device.productName);
    console.log({
      vid: device.vendorId.toString(16),
      pid: device.productId.toString(16),
    });

    await device.open();

    device.addEventListener("inputreport", (e) => {
      const rcvBytes = [...new Uint8Array(e.data.buffer)];
      // console.log({ e });
      console.log({ rcvBytes });
    });

    device.addEventListener("closed", () => {
      console.log("closed");
    });

    console.log("device opened");

    window.addEventListener("mousedown", (e) => {
      if (e.button === 2) {
        console.log("send");
        const buf = new Uint8Array(63);
        buf[0] = 10;
        buf[1] = 20;
        buf[2] = 30;
        const rawHidReportId = 2; //todo: get from collection
        device.sendReport(rawHidReportId, buf);
      }
    });
  }
}

window.addEventListener("click", start, { once: true });
window.addEventListener("contextmenu", (e) => e.preventDefault());