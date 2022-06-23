import 'package:flutter/foundation.dart';

@immutable
class Measurement {
  Measurement({
    required this.humidity,
    required this.temperature,
  });

  Measurement.fromJson(Map<String, Object?> json)
      : this(
          humidity: json['humidity']! as double,
          temperature: json['temperature']! as double,
        );

  final double humidity;
  final double temperature;

  Map<String, Object?> toJson() {
    return {
      'humidity': humidity,
      'temperature': temperature,
    };
  }
}
