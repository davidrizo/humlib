---
layout: default
breadcrumbs: [
                ['/',             'home'],
                ['/doc/class',    'classes'],
                ['/doc/snippet',  'snippets'],
                ['/doc/example',  'examples'],
                ['/doc/topic',    'topics'],
                ['/doc/tutorial', 'tutorials'],
                ['/doc/ref',      'reference']
        ]
title: Tutorials
---


{% include tutorials/tutorials.html %}

<p>
[<span style="cursor:pointer; color:#1e6bb8;" onclick="openAllDetails()">Open all</span>]
[<span style="cursor:pointer; color:#1e6bb8;" onclick="closeAllDetails()">Close all</span>]
</p>


<details class="tutorial-start">
<summary>
Getting started
</summary>
{% include tutorials/start.md %}
</details>

<details class="tutorial-read">
<summary>
Reading Humdrum data
</summary>
{% include tutorials/read.md %}
</details>

