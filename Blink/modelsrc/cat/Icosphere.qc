$modelname	"Icosphere.mdl"
$body main ".\Icosphere"
$staticprop
$surfaceprop	combine_metal
$cdmaterials	"models\props_sdk"

$sequence idle	"Icosphere-ref.smd"

$collisionmodel	"Icosphere-phys.smd" { $concave }