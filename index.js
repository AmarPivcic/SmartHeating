import { initializeApp } from "https://www.gstatic.com/firebasejs/10.5.2/firebase-app.js";

const firebaseConfig = {
    apiKey: "AIzaSyBN53IxSp887GFfnfqTtjnVV6i01cT0spo",
    authDomain: "smart-heating-11b6f.firebaseapp.com",
    databaseURL: "https://smart-heating-11b6f-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "smart-heating-11b6f",
    storageBucket: "smart-heating-11b6f.appspot.com",
    messagingSenderId: "998820917243",
    appId: "1:998820917243:web:d823e09c995cdece85a55e"
};

const app = initializeApp(firebaseConfig);

import {
    getDatabase,
    ref,
    child,
    update,
    set,
    get,
  } from "https://www.gstatic.com/firebasejs/10.5.2/firebase-database.js";

  const db = getDatabase();

  var temperature = document.getElementById("temperature");
  var humidity = document.getElementById("humidity");
  var setting = document.getElementById("setting");
  var dec = document.getElementById("dec");
  var inc = document.getElementById("inc");
  var modeTxt = document.getElementById("modeText");
  var wantedTemp = document.getElementById("wantedTemp");
  var decTemp = document.getElementById("decTemp");
  var incTemp = document.getElementById("incTemp");
  var switchBox = document.getElementById("switch");
  var radiatorImg = document.getElementById("radiatorImg");
  var temperatureValue=document.getElementById("temperatureValue");

  inc.addEventListener("click", increase);
  dec.addEventListener("click", decrease);
  incTemp.addEventListener("click", increaseTemp);
  decTemp.addEventListener("click", decreaseTemp);
  switchBox.addEventListener("click", changeMode);
  wantedTemp.addEventListener("change", newTemp);

  function newSetting() {
    update(ref(db, "values/"), {
      setting: parseInt(setting.value),
    })
      .catch((error) => {
        alert("error" + error);
      });
  }

  function changeMode() {
    if(modeTxt.innerHTML=="ON")
    {
      update(ref(db, "values/"), {
        auto: parseInt(0),
      })
        .catch((error) => {
          alert("error" + error);
        });
    }

    else
    {
      update(ref(db, "values/"), {
        auto: parseInt(1),
      })
        .catch((error) => {
          alert("error" + error);
        });
    }
  }

  function newTemp(){
    update(ref(db, "values/"), {
      wantedtemp: parseInt(wantedTemp.value),
    })
      .catch((error) => {
        alert("error" + error);
      });
  }

  function checkMode()
  {
    if(modeTxt.innerHTML=="ON")
    {
      inc.disabled=true;
      dec.disabled=true;
      incTemp.disabled=false;
      decTemp.disabled=false;
    }

    else{
      inc.disabled=false;
      dec.disabled=false;
      incTemp.disabled=true;
      decTemp.disabled=true;
    }
  }

  function setRotation()
  {
    if(setting.value==0)
    {
      radiatorImg.style.transform = `rotate(${90}deg)`;
    }

    else if(setting.value==1)
    {
      radiatorImg.style.transform = `rotate(${60}deg)`;
    }

    else if(setting.value==2)
    {
      radiatorImg.style.transform = `rotate(${30}deg)`;
    }

    else if(setting.value==3)
    {
      radiatorImg.style.transform = `rotate(${0}deg)`;
    }
    else if(setting.value==4)
    {
      radiatorImg.style.transform = `rotate(${-30}deg)`;
    }

    else if(setting.value==5)
    {
      radiatorImg.style.transform = `rotate(${-60}deg)`;
    }

    else if(setting.value==6)
    {
      radiatorImg.style.transform = `rotate(${-90}deg)`;
    }
  }


  function Load() {
    const dbRef = ref(db);

    get(child(dbRef, "values/")).then((snapshot)=>{
        if(snapshot.exists()) {
            temperature.innerHTML = "Temperature: " + snapshot.val().temp +" °C";
            humidity.innerHTML = "Humidity: " + snapshot.val().hum +"%";
            setting.value = snapshot.val().setting;
            wantedTemp.value = snapshot.val().wantedtemp;
            temperatureValue.innerHTML = wantedTemp.value +" °C";
            if(snapshot.val().auto == 1)
            {
              modeTxt.innerHTML = "ON";
              switchBox.checked = true;
            }
            else
            {
              modeTxt.innerHTML = "OFF";
              switchBox.checked = false;
            }
        }
        else {
            console.log("No data available");
          }
    }).catch((error) => {
        console.error(error);
      });
    checkMode();
    setRotation();
  }

  function decrease()
  {
    if(setting.value>0)
    {
        setting.value--;
    }
    newSetting();
  }

  function increase()
  {
    if(setting.value<6)
    {
        setting.value++;
    }
    newSetting();
  }

  function decreaseTemp()
  {
    if(wantedTemp.value>18)
    {
      wantedTemp.value--;
    }
    newTemp();
  }

  function increaseTemp()
  {
    if(wantedTemp.value<32)
    {
      wantedTemp.value++;
    }
    newTemp();
  }

  Load();
  window.setInterval(Load, 200);

