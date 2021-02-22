#include "HttpServer.h"
#include "ConfigManager.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

#ifndef ColorController_h
#define ColorController_h

class ColorController
{
public:
  static void getHome(Request *req, Response *res)
  {
    res->code = 200;
    res->contentType = "text/html";
    res->body = String("<html><head><title>RGB</title><style>.main-wrapper{margin:auto;width:500px}input[type=\"color\"]{padding:0;margin:0;border:none;height:40px;width:40px}input[type=\"color\"]::-webkit-color-swatch-wrapper{padding:0}input[type=\"color\"]::-webkit-color-swatch{border:none;border-radius:6px}.input-color-wrapper{display:flex;flex-direction:column}.input-color-wrapper .input-color-wrapper-type{display:block;font-family:Arial,Helvetica,sans-serif;font-weight:600;font-size:20px;padding:10px 0;border-bottom:1px solid #ddd}.input-color-wrapper .input-color-wrapper-type select{width:100%;height:50px;padding-left:10px;padding-right:10px;border:none;border-radius:6px;background-color:#eee;font-family:Arial,Helvetica,sans-serif;font-weight:600;font-size:15px}.input-field-wrapper{display:flex;flex-direction:column;padding:5px;margin:0;margin-top:50px}.input-field-wrapper .title{font-family:Arial,Helvetica,sans-serif;font-weight:700;font-size:17px}.input-fields{display:flex;flex-direction:row;align-items:center}.input-fields span{margin-left:10px;font-family:Arial,Helvetica,sans-serif;font-weight:600}.blocked{cursor:not-allowed;color:#888;background-color:#f5f5f5;border-radius:6px}.blocked input{cursor:not-allowed}.btn{padding:20px;margin:15px 0;width:200px;color:#fff;background-color:#0076e4;border:none;border-bottom:8px solid #0053a1;border-radius:6px;transition:all 0.3s;font-family:Arial,Helvetica,sans-serif;font-weight:600;font-size:15px}.btn:hover{cursor:pointer;background-color:#0053a1;margin-top:24px;border-bottom:0px solid #0053a1}.btn-cancel{background-color:#fa4222;border-color:#bd1d01}.btn-cancel:hover{background-color:#bd1d01}.submit-wrapper{margin-top:40px;display:flex;flex-direction:column;align-content:space-between;flex-wrap:wrap;height:90px}.slicer{width:100%}#speed-slider-value{width:30px;text-align:right;align-content:right}</style></head><body><div class=\"main-wrapper\"><div class=\"input-color-wrapper\"><div class=\"input-color-wrapper-type\"> <span class=\"title\">Primary color</span> <select id=\"input-animation\"><option value=\"1\" selected>Steady</option><option value=\"2\" disabled>Rainbow</option><option value=\"3\" disabled>Rain</option> </select></div><div class=\"input-field-wrapper\"> <span class=\"title\">Primary color</span><div class=\"input-fields\"> <input type=\"color\" id=\"primary-color\" class=\"color-picker\" value-ref-id=\"#primary-color-value\" name=\"primary-color\" value=\"#0076e4\"> <span id=\"primary-color-value\">#</span></div></div><div class=\"input-field-wrapper blocked\"> <span class=\"title\">Secondary color</span><div class=\"input-fields\"> <input disabled type=\"color\" id=\"secondary-color\" class=\"color-picker\" value-ref-id=\"#secondary-color-value\" name=\"secondary-color\" value=\"#0076e4\"> <span id=\"secondary-color-value\">#</span></div></div><div class=\"input-field-wrapper blocked\"> <span class=\"title\">Speed</span><div class=\"input-fields\"> <input disabled type=\"range\" min=\"0\" max=\"100\" value=\"50\" class=\"speed-slider slicer\" id=\"speed-slider\" value-ref-id=\"#speed-slider-value\"> <span id=\"speed-slider-value\">50%</span></div></div></div><div class=\"submit-wrapper\"> <button type=\"submit\" class=\"btn btn-cancel\">Cancel</button> <button type=\"submit\" class=\"btn btn-submit\">Apply</button></div></div></body> <script>function labelLister(input_query,id_ref_attribute,callback){let inputs=document.querySelectorAll(input_query);inputs.forEach(function(input){let label=document.querySelector(`${input.getAttribute(id_ref_attribute)}`);callback(input,label)();input.addEventListener('input',callback(input,label));});} window.onload=function(){labelLister(\".color-picker\",\"value-ref-id\",function(input,label){return function(){label.innerHTML=input.value;}});labelLister(\".slicer\",\"value-ref-id\",function(input,label){return function(){label.innerHTML=`${input.value}%`;}});let animation_type=document.querySelector(\"input-animation\");let primary_color=document.querySelector(\"primary-color\");let secondary_color=document.querySelector(\"secondary-color\");let speed=document.querySelector(\"speed-slider\");document.querySelector(\".btn-submit\").addEventListener('click',function(event){event.preventDefault();var xhr=new XMLHttpRequest();xhr.open(\"POST\",`http:xhr.setRequestHeader('Content-Type','application/json');xhr.send(JSON.stringify({animation_type:animation_type.value,primary_color:primary_color.value,secondary_color:secondary_color.value,speed:speed.value}));});document.querySelector(\".btn-cancel\").addEventListener('click',function(event){event.preventDefault();location.reload();});};</script> </html>");
  }

  static void postColor(Request *req, Response *res)
  {
    // Read the request body
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, req->body);
    JsonObject obj = doc.as<JsonObject>();

    req->configs->get().qtyLeafs = obj["qty_leaf"].as<uint16_t>();
    req->configs->get().qtyLedsPerLeaf = obj["qty_led_per_leaf"].as<uint8_t>();
    req->configs->get().mode = static_cast<Modes>(obj["mode"].as<uint8_t>());
    req->configs->get().speed = obj["speed"].as<uint8_t>();

    req->configs->get().primaryColor.r = obj["primary"][0].as<uint8_t>();
    req->configs->get().primaryColor.g = obj["primary"][1].as<uint8_t>();
    req->configs->get().primaryColor.b = obj["primary"][2].as<uint8_t>();

    req->configs->save();

    // Response
    res->code = 200;
    res->contentType = "application/json";
    res->body = String("{\"code\": 0, \"status\": \"Ok\"}");
  }
};

#endif