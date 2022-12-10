#![no_std]
#![no_main]
#![feature(type_alias_impl_trait)]

use embassy_executor::{main, task, Spawner};
use embassy_time::{Duration, Timer};

use esp32c3_hal::{
    clock::ClockControl, embassy, pac::Peripherals, prelude::*, timer::TimerGroup, Rtc,
};
use esp_backtrace as _;
use esp_println::println;

#[task]
async fn run1() {
    loop {
        println!("Hello world from embassy using esp-hal-async!");
        Timer::after(Duration::from_millis(1_000)).await;
    }
}

#[task]
async fn run2() {
    loop {
        println!("Bing!");
        Timer::after(Duration::from_millis(5_000)).await;
    }
}

#[main]
async fn main(spawner: Spawner) -> ! {
    let peripherals = Peripherals::take().unwrap();
    let system = peripherals.SYSTEM.split();
    let clocks = ClockControl::boot_defaults(system.clock_control).freeze();

    let mut rtc = Rtc::new(peripherals.RTC_CNTL);
    let timer_group0 = TimerGroup::new(peripherals.TIMG0, &clocks);
    let mut wdt0 = timer_group0.wdt;
    let timer_group1 = TimerGroup::new(peripherals.TIMG1, &clocks);
    let mut wdt1 = timer_group1.wdt;

    rtc.swd.disable();
    rtc.rwdt.disable();
    wdt0.disable();
    wdt1.disable();

    embassy::init(&clocks);

    spawner.spawn(run1()).ok();
    spawner.spawn(run2()).ok();
}
