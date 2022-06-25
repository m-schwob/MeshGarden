import 'package:chart_sparkline/chart_sparkline.dart';
import 'package:flutter/material.dart';
import 'package:iot_firestore_flutter_app/const/custom_colors.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:intl/intl.dart';
class MeasurementCard extends StatelessWidget {
  const MeasurementCard({
    Key? key,
    required this.value,
    required this.date_time,
    required this.name,
    required this.assetImage,
    required this.unit,
    // required this.trendData,
    // required this.linePoint
  }) : super(key: key);

  final num? value;
  final String? name;
  final String? unit;
  final DateTime? date_time;

  // final List<double> trendData;
  // final Color linePoint;
  final String assetImage;
  @override
  Widget build(BuildContext context) {
    return Card(
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(18),
        ),
        shadowColor: Colors.white,
        elevation: 24,
        color: kMainBG,
        child: Container(
          width: MediaQuery.of(context).size.width * 0.8,
          height: 250,
          child: Row(
            children: [
              Expanded(
                flex: 1,
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: [
                    Image(
                      width: 60,
                      image: AssetImage(
                        assetImage,
                      ),
                    ),
                    SizedBox(
                      height: 10,
                    ),
                    Text(name!, style: kBodyText.copyWith(color: Colors.white)),
                    SizedBox(
                      height: 10,
                    ),
                    Text('$value$unit',
                        style: kHeadline.copyWith(color: Colors.white)),
                    Text(DateFormat('dd-MM-yyy HH:mm:ss').format(date_time!), style: kSensorText.copyWith(color: Colors.grey)),
                  ],
                ),
              ),
              // Expanded(
              //   flex: 1,
              //   child: Container(
              //     padding:
              //     const EdgeInsets.symmetric(vertical: 30, horizontal: 8),
              //     child: Sparkline(
              //       data: trendData,
              //       lineWidth: 5.0,
              //       lineColor: Colors.white,
              //       averageLine: false,
              //       enableGridLines: true,
              //       gridLineLabelPrecision: 3,
              //       fillMode: FillMode.none,
              //       sharpCorners: false,
              //       pointsMode: PointsMode.last,
              //       pointSize: 20,
              //       pointColor: linePoint,
              //       useCubicSmoothing: true,
              //     ),
              //   ),
              // ),
            ],
          ),
        ));
  }
}
