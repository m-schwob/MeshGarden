import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:iot_firestore_flutter_app/model/Node.dart';
import 'package:iot_firestore_flutter_app/model/Sensor.dart';

class NodeService {

  FirebaseFirestore? _instance;

  List<Node> _nodes = []; //List of nodes

  List<Node> getNodes() {
    return _nodes;
  }

  Future<void> getNodesCollectionFromFirestore() async {
    _instance = FirebaseFirestore.instance;

    CollectionReference nodes_collection = _instance!.collection('TestNodes');

    // Get docs from collection reference
    QuerySnapshot nodes_querySnapshot = await nodes_collection.get();

    List<dynamic> nodes_list = nodes_querySnapshot.docs;



    // if (nodes_snapshot.exists) {
    //   Map<String, dynamic> data = snapshot.data() as Map<String, dynamic>;
    //   var nodesData = data['1234'] as List<dynamic>;
    //   nodesData.forEach((nodeData) {
    //     Node node = Node.fromJson(nodeData);
    //     _nodes.add(node);
    //   });
    // }
  }

  // void resetCategoriesToDefaults() {
  //   if (_categories.length > 0) {
  //     _categories.forEach((Category cat) {
  //       cat.subCategories!.forEach((Category subCat) {
  //
  //         (subCat as SubCategory).parts.forEach((CategoryPart part) {
  //           part.isSelected = false;
  //         });
  //
  //         (subCat as SubCategory).amount = 0;
  //       });
  //     });
  //   }
  // }
}