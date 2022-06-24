
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

class SensorMeasurements {
  String? type;
  List<Sample>? samples;
  String? units;
  Sample? newSample;

  SensorMeasurements({
    this.type,
    this.samples,
    this.units,
    this.newSample,
  });

  SensorMeasurements.fromJson(Map<String, Object?> json, String type)
      : this(
    type: type,
    samples: Sample.fromJsonArray(json['samples']! as List<dynamic>),
    units: SensorMeasurements.modify_units(json['units'] as String),
    newSample: Sample.fromNewSample(json['newSample']! as Map<String, Object?>),
  );

  static String modify_units(String units){
    String pretty_units = units.trim();
    if(units == "C"){
      pretty_units = "\'" + pretty_units;
    }
    return pretty_units;
  }
}


  class Sample {
    Timestamp? time;
    num? value;

    Sample({
      this.time,
      this.value,
    });

    Sample.fromJson(Map<String, Object?> json)
        : this(
      time: json['time'] as Timestamp,
      value: json['value'] as num,
    );

    static List<Sample> fromJsonArray(List<dynamic> jsonArray){
      List<Sample> samplesList= [];

      jsonArray.forEach((data) {
        samplesList.add(Sample.fromJson(data));
      });

      return samplesList;
    }

    static Sample fromNewSample(Map<String, Object?> sample_map){
      return Sample.fromJson(sample_map);
    }
  }

