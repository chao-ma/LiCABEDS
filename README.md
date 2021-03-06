<h2> An Ensemble Learning Algorithm and Implementation for Ligand Classification </h2>
<h4>Every geek has a dream</h4>

"Three heads are better than one". This old saying applies to machine learning as well. 
The very first time I got to know adaptive boosting, I was shocked by this magic algorithm: 
A set of "dumb" classifiers can be combined to build a "strong" classifier. 
Unlike support vector machine or classification tree, 
adaptive boosting is a framework for summing a set of "weak" classifiers together. 
Basically any robust classifier fits this framework, as long as it accepts weighted training data. 
Furthermore, it is quite simple to implement, easier than classification tree. 
Later, I borrowed this technique and applied to computer-aided drug design. 
A few chapters in my Ph.D. dissertation was on this topic. <a href="http://d-scholarship.pitt.edu/13904/1/thesis.pdf"> Link to my dissertation</a>. <p>

The basic idea is this: let's say we want to predict if a type of compound is soluble or not. 
People may say "-OH" group contributes to solubility. Other people say "low molecular weight indicates better solubility", etc. 
Obviously there are tons of counter examples for any of these arguments. But what if we build a rule combining all of them together? <br>
<strong>This is exactly what adaptive boosting is designed for. </strong>
Then I published two papers by following this idea:<p>

<h3><a href="http://pubs.acs.org/doi/abs/10.1021/ci100399j"> Ligand Classifier of Adaptively Boosting Ensemble Decision Stumps (LiCABEDS) and Its Application on Modeling Ligand Functionality for 5HT-Subtype GPCR Families </a></h3>
<img src="res/theme1.gif"><br>
<small>Advanced high-throughput screening (HTS) technologies generate great amounts of bioactivity data, and this data needs to be analyzed and interpreted with attention to understand how these small molecules affect biological systems. As such, there is an increasing demand to develop and adapt cheminformatics algorithms and tools in order to predict molecular and pharmacological properties on the basis of these large data sets. In this manuscript, we report a novel machine-learning-based ligand classification algorithm, named Ligand Classifier of Adaptively Boosting Ensemble Decision Stumps (LiCABEDS), for data-mining and modeling of large chemical data sets to predict pharmacological properties in an efficient and accurate manner. The performance of LiCABEDS was evaluated through predicting GPCR ligand functionality (agonist or antagonist) using four different molecular fingerprints, including Maccs, FP2, Unity, and Molprint 2D fingerprints. Our studies showed that LiCABEDS outperformed two other popular techniques, classification tree and Naive Bayes classifier, on all four types of molecular fingerprints. Parameters in LiCABEDS, including the number of boosting iterations, initialization condition, and a "reject option" boundary, were thoroughly explored and discussed to demonstrate the capability of handling imbalanced data sets, as well as its robustness and flexibility. In addition, the detailed mathematical concepts and theory are also given to address the principle behind statistical prediction models. The LiCABEDS algorithm has been implemented into a user-friendly software package that is accessible online at http://www.cbligand.org/LiCABEDS/.</small><p>
<h3><a href="http://pubs.acs.org/doi/abs/10.1021/ci3003914"> LiCABEDS II. Modeling of Ligand Selectivity for G-Protein-Coupled Cannabinoid Receptors</a></h3>
<img src="res/theme2.gif"><br>
<small>The cannabinoid receptor subtype 2 (CB2) is a promising therapeutic target for blood cancer, pain relief, osteoporosis, and immune system disease. The recent withdrawal of Rimonabant, which targets another closely related cannabinoid receptor (CB1), accentuates the importance of selectivity for the development of CB2 ligands in order to minimize their effects on the CB1 receptor. In our previous study, LiCABEDS (Ligand Classifier of Adaptively Boosting Ensemble Decision Stumps) was reported as a generic ligand classification algorithm for the prediction of categorical molecular properties. Here, we report extension of the application of LiCABEDS to the modeling of cannabinoid ligand selectivity with molecular fingerprints as descriptors. The performance of LiCABEDS was systematically compared with another popular classification algorithm, support vector machine (SVM), according to prediction precision and recall rate. In addition, the examination of LiCABEDS models revealed the difference in structure diversity of CB1 and CB2 selective ligands. The structure determination from data mining could be useful for the design of novel cannabinoid lead compounds. More importantly, the potential of LiCABEDS was demonstrated through successful identification of newly synthesized CB2 selective compounds.</small><p>

<strong>I am not talking about math in adaptive boosting or 5HT receptors here as the visitors to github are mostly interested in coding.</strong> I am going to talk about the software I built. Screenshots first:<p>
<table border="0">
    <tr><td><img src="res/capture1.jpg" height="213" width="431"></td><td><img src="res/capture2.jpg" height="213" width="431"></td></tr>
    <tr><td><img src="res/capture3.jpg" height="213" width="431"></td><td><img src="res/capture4.jpg" height="213" width="431"></td></tr>
</table>
<p>
I programmed all graphic user interface including start-up page (upper left), 
real-time training error curve (upper right), prediction overview (bottom left), and misc. dialogues (bottom right). 
There are a bunch of features as well, including workspace serialization, printing, sorting predicted labels, etc. 
Furthermore, the technology I used MFC (Microsoft Foundation Class). It was a pretty low-level Windows API wrapper for GUIs. That time, I was super interested in how to split window, how to create a custom window and respond to input events, how to print, etc. 
I think it is good learning experience as these days people use HTML or Android XML Layout to program interface, I see through how it works. So technically I can program my own view and build a fully customized component. 
Still don't believe I enjoyed it? I even wrote a user manual for my software: <a href="res/manual.pdf">software manual (pdf)</a>. And I did all the graphics design. <p>

What I learnt through this includes:<br>
 - Implementing adaptive boosting from scratch<br>
 - Implementing merge sort for the first time<br>
 - Cascading object serialization<br>
 - WYSIWYG printing<br>
 - Multithreaded programming: one thread does training, another thread does graphics.<p>

"Every geek has dream". I dreamed as well. I dived into MFC and C++, played with machine learning algorithms, designed user interface and tried to build my own software. This is just one attempt to fulfill the dream.
