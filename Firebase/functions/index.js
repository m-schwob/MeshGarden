const functions = require("firebase-functions");
const admin = require("firebase-admin");
admin.initializeApp();

exports.onNodeWrite = functions.firestore.document('/Nodes/{node_id}').onWrite(async (change, context) => {
    const node_id = context.params.node_id;

    if (change.after.exists) {
        const data = change.after.data();
        const doc = admin.firestore().collection('Changes').doc(node_id);
        
        doc.set({ 'config': JSON.stringify(data) });
    } else {
        functions.logger.log("Node " + node_id + "has deleted");
    }

    const doc = admin.firestore().collection('Changes').doc('10');
    const config = await doc.get(); 
    console.log(config.data());
});
