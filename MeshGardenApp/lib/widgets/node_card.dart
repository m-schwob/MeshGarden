//import 'package:chart_sparkline/chart_sparkline.dart';
import 'package:flutter/material.dart';
import 'package:iot_firestore_flutter_app/const/custom_colors.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';

class NodeCard extends StatelessWidget {
  const NodeCard(
      {Key? key,
        required this.nickname,
        required this.active,})
      : super(key: key);

  final String nickname;
  final bool active;
  //final List<double> trendData;
  //final Color linePoint;
  //final AssetImage assetImage;

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
          height: 200,
          child: Row(
            children: [
              Expanded(
                flex: 1,
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: [
                    SizedBox(
                      height: 10,
                    ),
                    Text(nickname, style: kBodyText.copyWith(color: Colors.white)),
                    SizedBox(
                      width: 10,
                    ),
                    Icon(
                      Icons.circle_outlined,
                      color: active? Colors.green : Colors.red
                    )
                    // Text('$value$unit',
                    //     style: kHeadline.copyWith(color: Colors.white)),
                  ],
                ),
              ),
            ],
          ),
        ));
  }
}
